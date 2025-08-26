#pragma once 

#include "command.hpp"

#include <unordered_map>
#include <string>
#include <optional>

namespace netfin::cli {
  struct ParseResult {
    const Command* command{};
    std::unordered_map<const Option*, std::string> options;
  };

  class ArgParser {
    public:
      ArgParser() = default;
      std::optional<ParseResult> parse(int argc, char** argv);
  };
}