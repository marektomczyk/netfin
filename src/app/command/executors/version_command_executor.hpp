#pragma once

#include "app/command/command_executor.hpp"

namespace netfin::app {
  class VersionCommandExecutor : public CommandExecutor {
    public:
    VersionCommandExecutor() : CommandExecutor(&cli::VERSION_CMD) {}
      virtual ~VersionCommandExecutor() = default;

      core::ErrorCode run(
        const std::unordered_map<const cli::Option*, std::string>& options, 
        ExecutorContext& context
      ) const override;
  };
}