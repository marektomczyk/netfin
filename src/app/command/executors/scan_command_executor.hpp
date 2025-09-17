#pragma once

#include "app/command/command_executor.hpp"

namespace netfin::app {
  class ScanCommandExecutor : public CommandExecutor {
    public:
    ScanCommandExecutor() : CommandExecutor(&cli::SCAN_CMD) {}
      virtual ~ScanCommandExecutor() = default;

      core::ErrorCode run(
        const ExecutorArgs& args, 
        ExecutorContext& context
      ) const override;
  };
}