#pragma once

#include "network/host_prober/host_prober.hpp"

namespace netfin::network {
  class HostProberWindows : public HostProber {
    public:
      ~HostProberWindows() = default;

      bool probe(
        std::string_view host,
        const std::chrono::milliseconds& timeout
      ) const override;
  };
}