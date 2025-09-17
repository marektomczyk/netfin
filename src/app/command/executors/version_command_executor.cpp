#include "version_command_executor.hpp"

#include "cli/option.hpp"
#include "core/error_code.hpp"
#include "core/version.hpp"

#include <unordered_map>
#include <string>
#include <sstream>

namespace netfin::app {
  core::ErrorCode VersionCommandExecutor::run(
    const std::unordered_map<const cli::Option*, std::string>& options, 
    ExecutorContext& context
  ) const {
    std::stringstream ss;
    ss << "netfin - " << core::APP_VERSION << "\n";
    context.out << ss.str();
    return core::ErrorCode::Success;
  }
}