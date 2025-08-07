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
}