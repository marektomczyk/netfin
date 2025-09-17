#include "host_prober_apple.hpp"
#include "core/platform.hpp"

#if NETFIN_PLATFORM_APPLE

#include "core/utils/utils.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/route.h>
#include <net/if_dl.h>
#include <unistd.h>

#include <thread>
#include <cstdint>
#include <chrono>

namespace netfin::core::host {

  bool HostProberApple::probe(
    std::string_view host,
    const std::chrono::milliseconds& timeout
  ) const noexcept {
    struct sockaddr_in addr{};
    if (!utils::resolve_ipv4(host, addr)) return false;

    if (has_arp_entry_for(addr.sin_addr.s_addr)) {
      return true;
    }

    int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) { 
      return false; 
    }

    fcntl(fd, F_SETFL, O_NONBLOCK);

    addr.sin_port = htons(9);

    const auto start = std::chrono::steady_clock::now();
    const auto deadline = start + timeout;

    const char b = '\0';
    while (std::chrono::steady_clock::now() < deadline) {
      ::sendto(fd, &b, 1, 0, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));

      std::this_thread::sleep_for(std::chrono::milliseconds(30));

      if (has_arp_entry_for(addr.sin_addr.s_addr)) {
        ::close(fd);
        return true;
      }
    }

    ::close(fd);
    return false;
  }

  bool HostProberApple::has_arp_entry_for(in_addr_t target) const noexcept {
    constexpr size_t mib_size = 6;
    int mib[mib_size];
    {
      int i = 0;
      mib[i++] = CTL_NET;
      mib[i++] = PF_ROUTE;
      mib[i++] = 0;
      mib[i++] = AF_INET;
      mib[i++] = NET_RT_FLAGS;
      mib[i++] = RTF_LLINFO;
    }

    size_t needed = 0;
    if (sysctl(mib, mib_size, nullptr, &needed, nullptr, 0) != 0 || needed == 0) {
      return false;
    }

    std::vector<unsigned char> buf(needed);
    if (sysctl(mib, mib_size, buf.data(), &needed, nullptr, 0) != 0) {
      return false;
    }

    unsigned char* next = buf.data();
    unsigned char* end  = buf.data() + needed;
    while (next < end) {
      auto* rtm = reinterpret_cast<struct rt_msghdr*>(next);
      if (rtm->rtm_msglen == 0) break;

      struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(rtm + 1);
      struct sockaddr_in* sin = nullptr;
      struct sockaddr_dl* sdl = nullptr;

      for (int i = 0, bit = 1; 
            i < RTAX_MAX && reinterpret_cast<unsigned char*>(sa) < next + rtm->rtm_msglen; 
            ++i, bit <<= 1) {

        if ((rtm->rtm_addrs & bit) == 0) continue;

        if (i == RTAX_DST) {
          if (sa->sa_family == AF_INET) sin = reinterpret_cast<struct sockaddr_in*>(sa);
        } else if (i == RTAX_GATEWAY) {
          if (sa->sa_family == AF_LINK) sdl = reinterpret_cast<struct sockaddr_dl*>(sa);
        }
        auto sa_size = sa->sa_len ? (1 + ((sa->sa_len - 1) | (sizeof(long) - 1))) : sizeof(long);
        sa = reinterpret_cast<struct sockaddr*>(reinterpret_cast<unsigned char*>(sa) + sa_size);
      }

      if (sin != nullptr && 
          sdl != nullptr && 
          sin->sin_addr.s_addr == target && 
          sdl->sdl_alen > 0) {
        return true;
      }

      next += rtm->rtm_msglen;
    }

    return false;
  }
}

#endif