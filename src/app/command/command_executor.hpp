
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

  class CommandExecutor {
    public:
      CommandExecutor(const cli::Command* command) : m_command(command) {}
      virtual ~CommandExecutor() = default;

      core::ErrorCode execute(
        const std::unordered_map<const cli::Option*, std::string>& options,
        ExecutorContext& context
      ) const;

      inline cli::CommandType command_type() const { return m_command->type; }

    protected:
      virtual core::ErrorCode run(
        const std::unordered_map<const cli::Option*, std::string>& options,
        ExecutorContext& context
      ) const = 0;

      inline const cli::Command* command() const { return m_command; }
    
      bool has_option(
        const std::unordered_map<const cli::Option*, 
        std::string>& options, cli::OptionType type
      ) const;

      std::variant<std::string, int, bool> get_option_value(
        const std::unordered_map<const cli::Option*, 
        std::string>& options, cli::OptionType type
      ) const;

      const cli::Command* m_command;

    private:
      void execute_help(ExecutorContext& context) const;
  };
}