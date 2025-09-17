#include "interface_finder.hpp"
#include "core/utils/utils.hpp"

#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

namespace netfin::core::network {
  std::vector<Interface> InterfaceFinder::find() const noexcept {
    std::vector<Interface> interfaces;

    struct ifaddrs* ifap;
    getifaddrs(&ifap);

    for (struct ifaddrs* p = ifap; p != nullptr; p = p->ifa_next) {
      if (p->ifa_addr && p->ifa_addr->sa_family == AF_INET) {
        char addr_buf[INET_ADDRSTRLEN];
        char netmask_buf[INET_ADDRSTRLEN];
        void* addr_ptr = &((struct sockaddr_in*)p->ifa_addr)->sin_addr;
        void* netmask_ptr = &((struct sockaddr_in*)p->ifa_netmask)->sin_addr;

        inet_ntop(AF_INET, addr_ptr, addr_buf, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, netmask_ptr, netmask_buf, INET_ADDRSTRLEN);

        interfaces.emplace_back(
          p->ifa_name,
          std::string(addr_buf),
          std::string(netmask_buf)
        );
      }
    }

    freeifaddrs(ifap);
    return interfaces;
  }
}
