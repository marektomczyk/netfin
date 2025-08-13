#pragma once

#include "network/host_prober/host_prober.hpp"
#include <netinet/in.h>

namespace netfin::network {
  class HostProberLinux : public HostProber {
    public:
      ~HostProberLinux() = default;

      bool probe(
        std::string_view host,
        const std::chrono::milliseconds& timeout
      ) const override;

    private:
      bool dgram_icmp_probe(
        const sockaddr_in& addr, 
        const std::chrono::milliseconds& timeout
      ) const;

      bool raw_icmp_probe(
        const sockaddr_in& addr, 
        const std::chrono::milliseconds& timeout
      ) const;

      bool tcp_probe(
        const sockaddr_in& addr, 
        const std::chrono::milliseconds& timeout
      ) const;
  };
}