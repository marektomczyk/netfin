#include "network/host_prober/host_prober.hpp"
#include "core/platform.hpp"

#if NETFIN_PLATFORM_WINDOWS
  #include "network/host_prober/host_prober_windows.hpp"
#elif NETFIN_PLATFORM_APPLE
  #include "network/host_prober/host_prober_apple.hpp"
#elif NETFIN_PLATFORM_LINUX
  #include "network/host_prober/host_prober_linux.hpp"
#endif

namespace netfin::network {
  std::unique_ptr<HostProber> HostProber::create() {
    #if NETFIN_PLATFORM_WINDOWS
      return std::make_unique<HostProberWindows>();
    #elif NETFIN_PLATFORM_APPLE
      return std::make_unique<HostProberApple>();
    #elif NETFIN_PLATFORM_LINUX
      return std::make_unique<HostProberLinux>();
    #else
      return nullptr;
    #endif
  }
}