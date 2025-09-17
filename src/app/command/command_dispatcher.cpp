#include "command_dispatcher.hpp"

#include "cli/command.hpp"
#include "core/error_code.hpp"
#include "executors/help_command_executor.hpp"
#include "executors/version_command_executor.hpp"
#include "executors/scan_command_executor.hpp"

#include <unordered_map>
#include <string>

namespace netfin::app {

  CommandDispatcher::CommandDispatcher() {
      register_executor(std::make_unique<HelpCommandExecutor>());
      register_executor(std::make_unique<VersionCommandExecutor>());
      register_executor(std::make_unique<ScanCommandExecutor>());
  }

  core::ErrorCode CommandDispatcher::dispatch(
    const cli::Command* command, 
    const ExecutorArgs& args, 
    ExecutorContext& context
  ) const {
    auto executor = m_command_executors.find(command->type);
    if (executor == m_command_executors.end()) {
      return core::ErrorCode::CommandNotFound;
    }
    return executor->second->execute(args, context);
  }

  void CommandDispatcher::register_executor(std::unique_ptr<CommandExecutor> executor) {
    m_command_executors.emplace(executor->command_type(), std::move(executor));
  }
} 