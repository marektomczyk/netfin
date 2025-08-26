#include <iostream>
#include <memory>

#include "network/interface_finder.hpp"
#include "network/host_prober/host_prober.hpp"
#include "core/error_code.hpp"
#include "cli/arg_parser.hpp"

int main(int argc, char** argv) {
    using namespace netfin;

    cli::ArgParser arg_parser;
    auto result = arg_parser.parse(argc, argv);

    if (result) {
        std::cout << "Command: " << result->command->name << '\n';
        for (const auto& [opt, value] : result->options) {
            std::cout << "Option: " << opt->name << " = " << value << '\n';
        }
        return 0;
    } else {
        std::cerr << "Invalid arguments\n";
        return 1;
    }

    // TODO(mt): will be refactored later.
    auto interface_finder = network::InterfaceFinder();
    auto interfaces = interface_finder.find();
    if (interfaces.empty()) {
        std::cerr << "No network interfaces found\n";
        return core::ErrorCode::NoNetworkInterfacesFound;
    }

    std::unique_ptr<network::HostProber> host_prober = network::HostProber::create();
    if (host_prober == nullptr) {
        std::cerr << "Platform not supported\n";
        return core::ErrorCode::PlatformNotSupported;
    }

    for (const auto& interface : interfaces) {
        if (interface.is_localhost()) continue;
        
        auto ip_addresses = interface.enumerate_ip_addresses();
        std::cout << "- " << interface.name() << ": " << interface.address() << '\n';
        for (const auto& ip : ip_addresses) {
            auto result = host_prober->probe(ip);
            if (result) {
                std::cout << " -> " << ip << " is REACHABLE\n";
            } else {
                std::cout << " -> " << ip << " is unreachable\n";
            }
        }
    }

    return core::ErrorCode::Success;
}