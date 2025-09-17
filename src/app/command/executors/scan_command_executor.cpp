#include "scan_command_executor.hpp"

#include "cli/option.hpp"
#include "core/error_code.hpp"
#include "core/version.hpp"
#include "core/network/interface_finder.hpp"
#include "core/host/host_prober_factory.hpp"

#include <unordered_map>
#include <string>
#include <sstream>

namespace netfin::app {
  core::ErrorCode ScanCommandExecutor::run(
    const std::unordered_map<const cli::Option*, std::string>& options, 
    ExecutorContext& context
  ) const {
    using namespace netfin::core::network;
    using namespace netfin::core::host;

    auto& host_prober = context.host_prober;
    auto& interface_finder = context.interface_finder;

    auto interfaces = interface_finder.find();
    if (interfaces.empty()) {
      context.err << "No network interfaces found\n";
      return core::ErrorCode::NoNetworkInterfacesFound;
    }

    const auto interface_var = get_option_value(options, cli::OptionType::Interface);
    if (auto interface = std::get_if<std::string>(&interface_var)) {
      std::erase_if(interfaces, [interface] (const auto& iface) {
        return iface.name() != *interface;
      });

      if (interfaces.empty()) {
        context.err << "Interface \"" << *interface << "\" not exists\n";
        return core::ErrorCode::InvalidOption;
      } else if (interfaces[0].is_localhost()) {
        context.err << "Interface is localhost, cannot be used\n";
        return core::ErrorCode::InvalidOption;
      }
    }
    

    auto timeout = HostProber::DEFAULT_TIMEOUT;
    const auto timeout_var = get_option_value(options, cli::OptionType::Timeout);
    if (auto tmo = std::get_if<int>(&timeout_var)) {
      if (*tmo <= 0) {
        context.err << "Timeout must be greater than 0\n";
        return core::ErrorCode::InvalidOption;
      }
      timeout = std::chrono::milliseconds(*tmo);
    }

    context.out << "* scanning " << interfaces.size() << " interface(s)\n";
    for (const auto& interface : interfaces) {
      context.out << "  - " << interface.name() << " (address: " << interface.address() << ")\n";
    }
    context.out << "* timeout for ping: " << timeout.count() << "ms\n";


    for (const auto& interface : interfaces) {
      if (interface.is_localhost()) continue;
      
      auto ip_addresses = interface.enumerate_ip_addresses();
      for (const auto& ip : ip_addresses) {
        if (ip == interface.address()) continue;
        auto result = host_prober.probe(ip, timeout);
        if (result) {
          context.out << "  -> " << ip << ": REACHABLE\n";
        } else {
          context.out << "  -> " << ip << ": unreachable\n";
        }
      }
    }

    return core::ErrorCode::Success;
  }

}