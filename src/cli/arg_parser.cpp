#include "arg_parser.hpp"

#include <string_view>
#include <iostream>
#include <string>

namespace netfin::cli {
  std::optional<ParseResult> ArgParser::parse(int argc, char** argv) {
    if (argc < 2) return std::nullopt;

    // Extract command name
    std::string_view first = argv[1];
    std::string_view command_name = first;
    if (auto sp = first.find(' '); sp != std::string::npos) {
      command_name = first.substr(0, sp);
    }

    // Find command definition
    const Command* command = nullptr;
    for (const auto& cmd : ALL_AVAILABLE_COMMANDS) {
      if (command_name == cmd.name) { command = &cmd; break; }
    }
    if (!command) return std::nullopt;

    // Collect tokens for options
    std::vector<std::string_view> tokens;
    for (int i = 2; i < argc; ++i) {
      if (argv[i] != nullptr && *argv[i] != '\0') tokens.emplace_back(argv[i]);
    }

    // Parse tokens
    std::unordered_map<const Option*, std::string> options;
    auto match_option = [&](std::string_view name) -> const Option* {
      // strip leading dashes if still present
      if (name.size() >= 2 && name[0] == '-' && name[1] == '-') name.remove_prefix(2);
      else if (!name.empty() && name[0] == '-') name.remove_prefix(1);
      for (const auto& opt : command->options) {
        if (name == opt.name || (!opt.shortcut.empty() && name == opt.shortcut)) {
          return &opt;
        }
      }
      return nullptr;
    };

    for (size_t i = 0; i < tokens.size(); ++i) {
      std::string_view tok = tokens[i];
      if (tok.size() == 0) continue;
      if (tok[0] != '-') {
        // positional or unsupported at this stage – skip for now
        continue;
      }

      // Handle --name=value or -n=value
      if (auto eq = tok.find('='); eq != std::string::npos) {
        std::string_view name = tok.substr(0, eq);
        std::string_view value = tok.substr(eq + 1);
        const Option* opt = match_option(name);
        if (!opt) return std::nullopt; // unknown option
        options[opt] = std::string(value);
        continue;
      }

      // Handle --name value (peek next token)
      const Option* opt = match_option(tok);
      if (!opt) return std::nullopt; // unknown option

      std::string_view value;
      if (i + 1 < tokens.size() && !tokens[i + 1].empty() && tokens[i + 1][0] != '-') {
        value = tokens[i + 1];
        ++i; 
      } else {
        // flag without explicit value
        value = std::string_view{"true"};
      }
      options[opt] = std::string(value);
    }

    return ParseResult{ command, options };
  }
}