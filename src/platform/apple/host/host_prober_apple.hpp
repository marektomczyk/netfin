#pragma once

#include "core/host/host_prober.hpp"
#include <netinet/in.h>

namespace netfin::core::host {
  class HostProberApple : public HostProber {
    public:
      ~HostProberApple() = default;

      bool probe(
        std::string_view host,
        const std::chrono::milliseconds& timeout
      ) const noexcept override;

    private:
      bool has_arp_entry_for(in_addr_t target) const noexcept;
  };
}