#pragma once

#include "app/command/command_executor.hpp"

namespace netfin::app {
  class HelpCommandExecutor : public CommandExecutor {
    public:
      HelpCommandExecutor() : CommandExecutor(&cli::HELP_CMD) {}
      virtual ~HelpCommandExecutor() = default;

      core::ErrorCode run(
        const ExecutorArgs& args, 
        ExecutorContext& context
      ) const override;
  };
}