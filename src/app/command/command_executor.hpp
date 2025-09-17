
#pragma once

#include "cli/command.hpp"
#include "cli/option.hpp"
#include "core/host/host_prober.hpp"
#include "core/network/interface_finder.hpp"
#include "core/error_code.hpp"

#include <unordered_map>
#include <string>
#include <iostream>
#include <variant>

namespace netfin::app {

  struct ExecutorContext {
    core::host::HostProber& host_prober;
    core::network::InterfaceFinder& interface_finder;
    std::ostream& out = std::cout;
    std::ostream& err = std::cerr;
  };

  struct ExecutorArgs {
    const std::unordered_map<const cli::Option*, std::string>& options;

    bool has_option(cli::OptionType type) const noexcept {
      for (const auto& [opt, _] : options) {
        if (opt && opt->type == type) return true;
      }
      return false;
    }
 
    std::variant<std::string, int, bool> get_option_value(cli::OptionType type) const noexcept {
      for (const auto& [opt, value] : options) {
        if (opt != nullptr && opt->type == type) {
          switch (opt->dataType) {
            case cli::OptionDataType::String: return std::string(value);
            case cli::OptionDataType::Boolean: return value == "true" || value == "1";
            // TODO(mt): other number types?
            case cli::OptionDataType::Number: return std::stoi(value); 
            case cli::OptionDataType::None: return "";
          }
        }
      }
      return std::variant<std::string, int, bool>{};
    }
  };

  class CommandExecutor {
    public:
      CommandExecutor(const cli::Command* command) : m_command(command) {}
      virtual ~CommandExecutor() = default;

      core::ErrorCode execute(
        const ExecutorArgs& args,
        ExecutorContext& context
      ) const;

      inline cli::CommandType command_type() const { return m_command->type; }

    protected:
      virtual core::ErrorCode run(
        const ExecutorArgs& args,
        ExecutorContext& context
      ) const = 0;

      inline const cli::Command* command() const { return m_command; }

      const cli::Command* m_command;

    private:
      void execute_help(ExecutorContext& context) const;
  };
}