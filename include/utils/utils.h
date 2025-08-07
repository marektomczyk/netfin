#pragma once

#include <string>
#include <optional>
#include <cstdint>

namespace netfin::utils {
  std::optional<uint32_t> ip_to_uint32(const std::string_view& ip);
  std::optional<std::string> uint32_to_ip(uint32_t ip);
}