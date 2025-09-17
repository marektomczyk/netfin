#include "host_prober_factory.hpp"
#include "core/platform.hpp"

#if NETFIN_PLATFORM_WINDOWS
  #include "platform/windows/host/host_prober_windows.hpp"
#elif NETFIN_PLATFORM_APPLE
  #include "platform/apple/host/host_prober_apple.hpp"
#elif NETFIN_PLATFORM_LINUX
  #include "platform/linux/host/host_prober_linux.hpp"
#endif

namespace netfin::core::host {
  namespace HostProberFactory {
    std::unique_ptr<HostProber> create() noexcept {
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
}