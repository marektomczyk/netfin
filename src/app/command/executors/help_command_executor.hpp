#pragma once

#include "app/command/command_executor.hpp"

namespace netfin::app {
  class HelpCommandExecutor : public CommandExecutor {
    public:
      HelpCommandExecutor() : CommandExecutor(&cli::HELP_CMD) {}
      virtual ~HelpCommandExecutor() = default;

      core::ErrorCode run(
        const std::unordered_map<const cli::Option*, std::string>& options, 
        ExecutorContext& context
      ) const override;
  };
}