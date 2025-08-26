#pragma once

#include "host_prober.hpp"

#include <memory>

namespace netfin::core::host {
  namespace HostProberFactory {
    std::unique_ptr<HostProber> create();
  };
}