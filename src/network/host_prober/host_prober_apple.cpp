#include "network/host_prober/host_prober_apple.hpp"
#include "core/platform.hpp"

#if NETFIN_PLATFORM_APPLE

#include "utils/utils.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <cstdint>
#include <cerrno>
#include <cstring>
#include <chrono>
#include <array>
#include <future>

namespace netfin::network {

  bool HostProberApple::probe(
    std::string_view host,
    const std::chrono::milliseconds& timeout
  ) const {
    struct sockaddr_in addr{};
    if (!utils::resolve_ipv4(host, addr)) {
      return false;
    }

    auto fut_tcp = std::async(std::launch::async, [this, addr, timeout] {
      return this->tcp_probe(addr, timeout);
    });
    auto fut_udp = std::async(std::launch::async, [this, addr, timeout] {
      return this->udp_probe(addr, timeout);
    });

    const bool tcp_ok = fut_tcp.get();
    const bool udp_ok = fut_udp.get();

    return tcp_ok || udp_ok;
  }

  bool HostProberApple::tcp_probe(
    const sockaddr_in& addr, 
    const std::chrono::milliseconds& timeout
  ) const {
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    int flags = ::fcntl(sock, F_GETFL, 0);
    if (flags >= 0) ::fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in tcp_addr = addr;
    tcp_addr.sin_port = htons(443);

    int is_connected = ::connect(sock, reinterpret_cast<const sockaddr*>(&tcp_addr), sizeof(tcp_addr));
    if (is_connected == 0) { ::close(sock); return true; }
    if (is_connected < 0 && errno == EINPROGRESS) {
      struct pollfd pfd{
        .fd = sock,
        .events = POLLOUT, 
        .revents = 0
      }; 
      
      int poll_result = ::poll(&pfd, 1, static_cast<int>(timeout.count()));
      if (poll_result > 0) {
        int soerr = 0; 
        socklen_t slen = sizeof(soerr);
        ::getsockopt(sock, SOL_SOCKET, SO_ERROR, &soerr, &slen);
        ::close(sock);
        if (soerr == 0) return true;           
        if (soerr == ECONNREFUSED) return true;
        return false;                            
      }
    }
    ::close(sock);
    return false;
  }

  bool HostProberApple::udp_probe(
    const sockaddr_in& addr, 
    const std::chrono::milliseconds& timeout
  ) const {
    int sock = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return false;

    struct sockaddr_in udp_addr = addr; 
    udp_addr.sin_port = htons(33434);
    if (::connect(sock, reinterpret_cast<const sockaddr*>(&udp_addr), sizeof(udp_addr)) < 0) {
      ::close(sock); return false;
    }

    struct timeval tv{
      .tv_sec = static_cast<int>(timeout.count() / 1000),
      .tv_usec = static_cast<int>((timeout.count() % 1000) * 1000)
    }; 
    ::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    uint8_t b = 0; 
    ::send(sock, &b, 1, 0);

    uint8_t rb[1];
    ssize_t recv_result = ::recv(sock, rb, sizeof(rb), 0);
    if (recv_result < 0) {
      int err = errno;
      ::close(sock);
      if (err == ECONNREFUSED) return true;
      return false;
    }

    ::close(sock);
    return true; 
  }
}

#endif