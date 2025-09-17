#pragma once

#include "core/host/host_prober.hpp"

namespace netfin::core::host {
  class HostProberWindows : public HostProber {
    public:
      ~HostProberWindows() = default;

      bool probe(
        std::string_view host,
        const std::chrono::milliseconds& timeout
      ) const noexcept override;
  };
}