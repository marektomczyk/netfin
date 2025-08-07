#include <iostream>

#include "network/interface_finder.hpp"

int main() {
    auto interface_finder = netfin::network::InterfaceFinder();

    auto interfaces = interface_finder.find();

    for (const auto& interface : interfaces) {
        if (interface.is_localhost()) continue;
        
        auto ip_addresses = interface.enumerate_ip_addresses();
        std::cout << "- " << interface.name() << ": " << interface.address() << '\n';
        for (const auto& ip : ip_addresses) {
            std::cout << "  - " << ip << '\n';
        }
    }

    return 0;
}