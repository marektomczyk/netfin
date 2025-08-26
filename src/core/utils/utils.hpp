#pragma once

#include <string>
#include <optional>
#include <cstdint>
#include <netinet/in.h>

namespace netfin::utils {
  std::optional<uint32_t> ip_to_uint32(const std::string_view& ip);
  
  std::optional<std::string> uint32_to_ip(uint32_t ip);

  uint16_t checksum16(const void* data, size_t len);

  bool resolve_ipv4(std::string_view host, sockaddr_in& out);
}