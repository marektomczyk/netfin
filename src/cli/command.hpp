#pragma once

#include "option.hpp"

#include <string_view>
#include <span>
  
namespace netfin::cli {
  enum class CommandType {
    Scan,
    Help,
    Version,
    List,
  };

  struct Command {
    CommandType type;
    std::string_view name;
    std::string_view description;
    std::string_view usage;
    std::span<const Option> options;
  };
  
  inline constexpr Option SCAN_OPTS[] = { OPT_INTERFACE, OPT_TIMEOUT, OPT_HELP };
  inline constexpr Command SCAN_CMD = {
    CommandType::Scan,
    "scan",
    "Send ICMP echo requests to an IP or CIDR range",
    "netfin scan [options]",
    SCAN_OPTS,
  };
  inline constexpr Command HELP_CMD = {
    CommandType::Help,
    "help",
    "Show general help and usage information",
    "netfin help",
    {},
  };
  inline constexpr Command VERSION_CMD = {
    CommandType::Version,
    "version",
    "Show the application version",
    "netfin version",
    {},
  };

  inline constexpr Command ALL_AVAILABLE_COMMANDS[] = {
    SCAN_CMD,
    HELP_CMD,
    VERSION_CMD,
  };
}