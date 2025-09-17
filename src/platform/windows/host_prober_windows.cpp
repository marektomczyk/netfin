#include "host_prober_windows.hpp"
#include "core/platform.hpp"

#if NETFIN_PLATFORM_WINDOWS

namespace netfin::core::host {
  bool HostProberWindows::probe(
    std::string_view host,
    const std::chrono::milliseconds& timeout
  ) const noexcept {
    // TODO(mt): Implement windows prober
    return false;
  }
}

#endif  