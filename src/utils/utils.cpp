#include "utils/utils.h"
#include <arpa/inet.h>
#include <cassert>

namespace netfin::utils {
  std::optional<uint32_t> ip_to_uint32(const std::string_view& ip) {
    struct in_addr addr;
    int result = inet_pton(AF_INET, ip.data(), &addr);
    if (result != 1) return std::nullopt;
    return ntohl(addr.s_addr);
  }
  
  std::optional<std::string> uint32_to_ip(uint32_t ip) {
    struct in_addr addr;
    addr.s_addr = htonl(ip);
    char buffer[INET_ADDRSTRLEN];
    const char* result = inet_ntop(AF_INET, &addr, buffer, INET_ADDRSTRLEN);
    if (result == nullptr) return std::nullopt;
    return std::string(buffer);
  }

  uint16_t checksum16(const void* data, size_t len) {
    const uint16_t* w = static_cast<const uint16_t*>(data);
    uint32_t sum = 0;
    while (len > 1) {
      sum += *w++;
      len -= 2;
    }
    if (len == 1) {
      uint16_t last = 0;
      *reinterpret_cast<uint8_t*>(&last) = *reinterpret_cast<const uint8_t*>(w);
      sum += last;
    }
    while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    return static_cast<uint16_t>(~sum);
  }

  bool resolve_ipv4(std::string_view host, sockaddr_in& out) {
    std::memset(&out, 0, sizeof(out));
    out.sin_family = AF_INET;
    return ::inet_pton(AF_INET, std::string(host).c_str(), &out.sin_addr) == 1;
  }
}