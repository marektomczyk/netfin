#pragma once

#include <string>
#include <chrono>
#include <memory>

namespace netfin::core::host {
  class HostProber {
    public:
      virtual ~HostProber() = default;

      virtual bool probe(
        std::string_view host,
        const std::chrono::milliseconds& timeout = DEFAULT_TIMEOUT
      ) const noexcept = 0;

      inline static constexpr std::chrono::milliseconds DEFAULT_TIMEOUT 
        = std::chrono::milliseconds(1000);
  };
}