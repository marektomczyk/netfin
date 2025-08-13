#pragma once

#include <string>
#include <chrono>
#include <memory>

namespace netfin::network {
  class HostProber {
    public:
      virtual ~HostProber() = default;

      virtual bool probe(
        std::string_view host,
        const std::chrono::milliseconds& timeout = DEFAULT_TIMEOUT
      ) const = 0;

    private:
      inline static constexpr std::chrono::milliseconds DEFAULT_TIMEOUT 
        = std::chrono::milliseconds(1000);

    public:
      static std::unique_ptr<HostProber> create();
  };
}