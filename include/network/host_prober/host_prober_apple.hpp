#pragma once

#include "network/host_prober/host_prober.hpp"
#include <netinet/in.h>

namespace netfin::network {
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