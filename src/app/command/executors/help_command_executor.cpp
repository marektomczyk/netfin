#include "help_command_executor.hpp"

#include "cli/option.hpp"
#include "core/error_code.hpp"

#include <unordered_map>
#include <string>
#include <sstream>

namespace netfin::app {
  core::ErrorCode HelpCommandExecutor::run(
    const ExecutorArgs& args, 
    ExecutorContext& context  
  ) const {
    std::stringstream ss;
    ss << "Usage: netfin <command> [options]\n\n";
    ss << "Available commands:\n";

    size_t max_len = 0;
    for (const auto& cmd : cli::ALL_AVAILABLE_COMMANDS) {
        max_len = std::max(max_len, cmd.name.size());
    }

    for (const auto& cmd : cli::ALL_AVAILABLE_COMMANDS) {
        ss << "  " << cmd.name
          << std::string(max_len - cmd.name.size() + 4, ' ') 
          << cmd.description << "\n";
    }

    ss << "\nOptions:\n";
    ss << "  -h, --help     Show this help message and exit\n";

    context.out << ss.str();
    return core::ErrorCode::Success;
  }
}