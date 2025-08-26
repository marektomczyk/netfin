#pragma once

#include "option.hpp"

#include <string_view>
#include <span>

namespace netfin::cli {
  enum class CommandType {
    Ping,
    Help,
    Version,
    List,
  };

  struct Command {
    CommandType type;
    std::string_view name;
    std::string_view description;
    std::span<const Option> options;
  };
  
  inline constexpr Option PING_OPTS[] = { OPT_INTERFACE, OPT_TIMEOUT };
  inline constexpr Command PING_CMD = {
    CommandType::Ping,
    "ping",
    "Send ICMP echo to IPs/CIDR",
    PING_OPTS,
  };
  inline constexpr Command HELP_CMD = {
    CommandType::Help,
    "help",
    "Show general help",
    {},
  };
  inline constexpr Command VERSION_CMD = {
    CommandType::Version,
    "version",
    "Show the app version",
    {},
  };
  inline constexpr Command LIST_CMD = {
    CommandType::List,
    "list",
    "List all commands",
    {},
  };

  inline constexpr Command ALL_AVAILABLE_COMMANDS[] = {
    PING_CMD,
    HELP_CMD,
    VERSION_CMD,
    LIST_CMD,
  };
}