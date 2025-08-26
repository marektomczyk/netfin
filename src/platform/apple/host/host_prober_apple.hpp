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
      ) const override;

    private:
      bool tcp_probe(
        const sockaddr_in& addr, 
        const std::chrono::milliseconds& timeout
      ) const;

      bool udp_probe(
        const sockaddr_in& addr, 
        const std::chrono::milliseconds& timeout
      ) const;
  };
}