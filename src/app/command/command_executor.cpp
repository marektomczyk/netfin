#include "command_executor.hpp"

namespace netfin::app {

  core::ErrorCode CommandExecutor::execute(
    const std::unordered_map<const cli::Option*, std::string>& options,
    ExecutorContext& context
  ) const {
    bool has_help_opt = has_option(options, cli::OptionType::Help);
    if (has_help_opt) {
      execute_help(context);
      return core::ErrorCode::Success;
    }
    return run(options, context);
  }

  bool CommandExecutor::has_option(
    const std::unordered_map<const cli::Option*, 
    std::string>& options, cli::OptionType type
  ) const {
    for (const auto& [opt, _] : options) {
      if (opt && opt->type == type) return true;
    }
    return false;
  }

  std::variant<std::string, int, bool> CommandExecutor::get_option_value(
    const std::unordered_map<const cli::Option*, 
    std::string>& options, cli::OptionType type
  ) const {
    for (const auto& [opt, value] : options) {
      if (opt != nullptr && opt->type == type) {
        switch (opt->dataType) {
          case cli::OptionDataType::String: return std::string(value);
          case cli::OptionDataType::Boolean: return value == "true" || value == "1";
          // TODO(mt): other number types?
          case cli::OptionDataType::Number: return std::stoi(value); 
          case cli::OptionDataType::None: return "";
        }
      }
    }
    return std::variant<std::string, int, bool>{};
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