#pragma once

#include <string>
#include <vector>
#include "utils/utils.h"

namespace netfin::network {
  class Interface {
    public:
      explicit Interface(
        const std::string& name,
        const std::string& address,
        const std::string& netmask
      ) : m_name(name), m_address(address), m_netmask(netmask) {}

      ~Interface() = default;

      bool is_localhost() const {
        return m_address == LOCALHOST_IPV4 || m_address == LOCALHOST_IPV6;
      }

      std::string_view name() const { return m_name; }
      std::string_view address() const { return m_address; }
      std::string_view netmask() const { return m_netmask; }

      std::vector<std::string> enumerate_ip_addresses() const {
        std::vector<std::string> ip_addresses;

        auto address_opt = utils::ip_to_uint32(m_address);
        auto netmask_opt = utils::ip_to_uint32(m_netmask);

        if (!address_opt || !netmask_opt) return ip_addresses;

        auto address = *address_opt;
        auto netmask = *netmask_opt;

        auto network_address = address & netmask;
        auto broadcast_address = network_address | ~netmask;

        
        for (uint32_t ip = network_address; ip <= broadcast_address; ip++) {
          uint8_t last_octet = ip & 0xFF;
          if (last_octet == 0u || last_octet == 255u) continue;

          auto ip_str_opt = utils::uint32_to_ip(ip);
          if (ip_str_opt) ip_addresses.push_back(*ip_str_opt);
        }

        return ip_addresses;
      }

    private:
      std::string m_name;
      std::string m_address;
      std::string m_netmask;

      inline static constexpr std::string_view LOCALHOST_IPV4 
        = "127.0.0.1";
      inline static constexpr std::string_view LOCALHOST_IPV6 
        = "::1";
  };
}