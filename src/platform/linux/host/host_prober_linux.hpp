#pragma once

#include "core/host/host_prober.hpp"
#include <netinet/in.h>

namespace netfin::core::host {
  class HostProberLinux : public HostProber {
    public:
      ~HostProberLinux() = default;

      bool probe(
        std::string_view host,
        const std::chrono::milliseconds& timeout
      ) const noexcept override;

    private:
      bool dgram_icmp_probe(
        const sockaddr_in& addr, 
        const std::chrono::milliseconds& timeout
      ) const noexcept;

      bool raw_icmp_probe(
        const sockaddr_in& addr, 
        const std::chrono::milliseconds& timeout
      ) const noexcept;

      bool tcp_probe(
        const sockaddr_in& addr, 
        const std::chrono::milliseconds& timeout
      ) const noexcept;
  };
}