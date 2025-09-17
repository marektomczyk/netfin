#include "command_executor.hpp"

namespace netfin::app {

  core::ErrorCode CommandExecutor::execute(
    const ExecutorArgs& args,
    ExecutorContext& context
  ) const {
    bool has_help_opt = args.has_option(cli::OptionType::Help);
    if (has_help_opt) {
      execute_help(context);
      return core::ErrorCode::Success;
    }
    return run(args, context);
  }

  void CommandExecutor::execute_help(ExecutorContext& context) const {
    std::stringstream ss;
    ss << "Usage: " << m_command->usage << "\n\n";
    ss << "Options:\n";

    size_t max_len = 0;
    for (const auto& opt : m_command->options) {
      max_len = std::max(max_len, opt.name.size());
    }
    for (const auto& opt : m_command->options) {
      ss << "  " << opt.name
        << std::string(max_len - opt.name.size() + 4, ' ') 
        << opt.description << "\n";
    }
    context.out << ss.str();
  }
}