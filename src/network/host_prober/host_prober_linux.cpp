#include "network/host_prober/host_prober_linux.hpp"
#include "core/platform.hpp"

#if NETFIN_PLATFORM_LINUX

#include "utils/utils.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <cstdint>
#include <cerrno>
#include <cstring>
#include <chrono>
#include <future>
#include <fcntl.h>

namespace netfin::network {

  bool HostProberLinux::probe(
    std::string_view host,
    const std::chrono::milliseconds& timeout
  ) const {
    sockaddr_in addr{};
    if (!utils::resolve_ipv4(host, addr)) {
      return false;
    }

    if (dgram_icmp_probe(addr, timeout)) return true;
    if (raw_icmp_probe(addr, timeout)) return true;

    auto fut_tcp = std::async(std::launch::async, [this, addr, timeout] {
      return this->tcp_probe(addr, timeout);
    });

    auto fut_udp = std::async(std::launch::async, [this, addr, timeout] {
      return this->udp_probe(addr, timeout);
    });

    return fut_tcp.get() || fut_udp.get();
  }

  bool HostProberLinux::dgram_icmp_probe(
    const sockaddr_in& addr, 
    const std::chrono::milliseconds& timeout
  ) const {
    int icmp_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
    if (icmp_socket < 0) return false;

    uint16_t ident = static_cast<uint16_t>(::getpid() & 0xFFFF);
    struct icmphdr hdr{
      .type = ICMP_ECHO,
      .code = 0,
      .un.echo.id = htons(ident),
      .un.echo.sequence = htons(0),
      .checksum = 0,
    };
    hdr.checksum = utils::checksum16(&hdr, sizeof(hdr));

    if (::sendto(icmp_socket, &hdr, sizeof(hdr), 0, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) < 0) {
      ::close(icmp_socket); return false;
    }

    struct timeval tv{
      .tv_sec = static_cast<int>(timeout.count() / 1000),
      .tv_usec = static_cast<int>((timeout.count() % 1000) * 1000)
    };
    ::setsockopt(icmp_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    uint8_t buf[1500];
    ssize_t received_bytes = ::recv(icmp_socket, buf, sizeof(buf), 0);
    if (received_bytes < static_cast<ssize_t>(sizeof(struct icmphdr))) {
      ::close(icmp_socket); return false;
    }

    const struct icmphdr* ih = reinterpret_cast<const struct icmphdr*>(buf);
    bool ok = (ih->type == ICMP_ECHOREPLY) && (ih->code == 0) && (ntohs(ih->un.echo.id) == ident);
    ::close(icmp_socket);
    return ok;
  }

  bool HostProberLinux::raw_icmp_probe(
    const sockaddr_in& addr, 
    const std::chrono::milliseconds& timeout
  ) const {
    int icmp_socket = ::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (icmp_socket < 0) return false;

    const int ttl = 64;
    ::setsockopt(icmp_socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

    uint16_t ident = static_cast<uint16_t>(::getpid() & 0xFFFF);
    struct icmphdr hdr{
      .type = ICMP_ECHO,
      .code = 0,
      .un.echo.id = htons(ident),
      .un.echo.sequence = htons(0),
      .checksum = 0,
    };
    hdr.checksum = utils::checksum16(&hdr, sizeof(hdr));

    if (::sendto(icmp_socket, &hdr, sizeof(hdr), 0, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) < 0) {
      ::close(icmp_socket); return false;
    }

    struct pollfd pfd{
      .fd = icmp_socket,
      .events = POLLIN,
      .revents = 0
    };
    if (::poll(&pfd, 1, static_cast<int>(timeout.count())) <= 0) {
      ::close(icmp_socket); return false;
    }

    uint8_t buf[1500];
    ssize_t received_bytes = ::recv(icmp_socket, buf, sizeof(buf), 0);
    if (received_bytes < 20) {
      ::close(icmp_socket); return false;
    }

    const struct iphdr* ip_header = reinterpret_cast<const struct iphdr*>(buf);
    size_t iphdr_len = static_cast<size_t>(ip_header->ihl) * 4;
    if (received_bytes < static_cast<ssize_t>(iphdr_len + sizeof(struct icmphdr))) {
      ::close(icmp_socket); return false;
    }

    const struct icmphdr* ih = reinterpret_cast<const struct icmphdr*>(buf + iphdr_len);
    bool ok = (ih->type == ICMP_ECHOREPLY) && (ih->code == 0) && (ntohs(ih->un.echo.id) == ident);
    ::close(icmp_socket);
    return ok;
  }

  bool HostProberLinux::tcp_probe(
    const sockaddr_in& addr, 
    const std::chrono::milliseconds& timeout
  ) const {
    int tcp_socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket < 0) return false;

    int flags = ::fcntl(tcp_socket, F_GETFL, 0);
    if (flags >= 0) ::fcntl(tcp_socket, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in tcp_addr = addr;
    tcp_addr.sin_port = htons(443);

    int is_connected = ::connect(tcp_socket, reinterpret_cast<const sockaddr*>(&tcp_addr), sizeof(tcp_addr));
    if (is_connected == 0) { ::close(tcp_socket); return true; }
    if (is_connected < 0 && errno == EINPROGRESS) {
      struct pollfd pfd{}; pfd.fd = tcp_socket; pfd.events = POLLOUT;
      int poll_result = ::poll(&pfd, 1, static_cast<int>(timeout.count()));
      if (poll_result > 0) {
        int err = 0; 
        socklen_t slen = sizeof(err);
        ::getsockopt(tcp_socket, SOL_SOCKET, SO_ERROR, &err, &slen);
        ::close(tcp_socket);
        if (err == 0) return true;
        if (err == ECONNREFUSED) return true;
      }
    }
    ::close(tcp_socket);
    return false;
  }

  bool HostProberLinux::udp_probe(
    const sockaddr_in& addr, 
    const std::chrono::milliseconds& timeout
  ) const {
    int udp_socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) return false;

    struct sockaddr_in udp_addr = addr; 
    udp_addr.sin_port = htons(33434);
    if (::connect(udp_socket, reinterpret_cast<const sockaddr*>(&udp_addr), sizeof(udp_addr)) < 0) {
      ::close(udp_socket); return false;
    }

    struct timeval tv{
      .tv_sec = static_cast<int>(timeout.count() / 1000),
      .tv_usec = static_cast<int>((timeout.count() % 1000) * 1000)
    };
    ::setsockopt(udp_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    uint8_t b = 0;
    ::send(udp_socket, &b, 1, 0);

    uint8_t rb[1];
    ssize_t recv_result = ::recv(udp_socket, rb, sizeof(rb), 0);
    if (recv_result < 0) {
      int err = errno;
      ::close(udp_socket);
      if (err == ECONNREFUSED) return true;
      return false;
    }
    ::close(udp_socket);
    return true;
  }

} 
#endif // __linux__