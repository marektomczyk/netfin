#pragma once

#include "command_executor.hpp"
#include "executors/help_command_executor.hpp"

#include <unordered_map>
#include <memory>

namespace netfin::app {
  class CommandDispatcher {
    public:
      CommandDispatcher();
      virtual ~CommandDispatcher() = default;

      core::ErrorCode dispatch(
        const cli::Command* command, 
        const ExecutorArgs& args, 
        ExecutorContext& context
      ) const;

      private:
        std::unordered_map<cli::CommandType, std::unique_ptr<CommandExecutor>> m_command_executors;

        void register_executor(std::unique_ptr<CommandExecutor> executor);
  };
}