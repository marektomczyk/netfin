#pragma once

#include <string_view>

namespace netfin::cli {
  enum class OptionType {
    Help,
    Interface,
    Timeout,
  };

  enum class OptionDataType {
    None,
    Boolean,
    Number,
    String
  };

  struct Option {
      OptionType type;
      std::string_view name;
      std::string_view shortcut;
      std::string_view description;
      OptionDataType dataType;
  };

  inline constexpr Option OPT_HELP = { 
    OptionType::Help,
    "help", 
    "h", 
    "Show help for the command", 
    OptionDataType::None 
  };
  inline constexpr Option OPT_INTERFACE = { 
    OptionType::Interface,
    "interface", 
    "i", 
    "Select the interface to be used", 
    OptionDataType::String 
  };
  inline constexpr Option OPT_TIMEOUT = { 
    OptionType::Timeout,
    "timeout", 
    "t", 
    "Timeout in milliseconds for the single ping", 
    OptionDataType::Number 
  };

  inline constexpr Option ALL_AVAILABLE_OPTIONS[] = {
    OPT_HELP,
    OPT_INTERFACE,
    OPT_TIMEOUT,
  };
}