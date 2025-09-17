#include "core/network/interface_finder.hpp"
#include "core/host/host_prober_factory.hpp"
#include "core/error_code.hpp"
#include "cli/arg_parser.hpp"
#include "command/command_dispatcher.hpp"

#include <iostream>
#include <memory>
#include <signal.h>

int main(int argc, char** argv) {
    ::signal(SIGPIPE, SIG_IGN);

    using namespace netfin;
    using namespace netfin::app;
    using namespace netfin::core;
    using namespace netfin::cli;
    using namespace netfin::core::host;
    using namespace netfin::core::network;

    ArgParser arg_parser;
    auto parseResult = arg_parser.parse(argc, argv);

    std::ostream&  out = std::cout;
    std::ostream&  err = std::cerr;
    std::unique_ptr<HostProber> host_prober = HostProberFactory::create();
    InterfaceFinder interface_finder = InterfaceFinder();

    if (host_prober == nullptr) {
        err << "Platform not supported\n";
        return ErrorCode::PlatformNotSupported;
    }

    app::ExecutorContext executor_context = {
        .host_prober = *host_prober,
        .interface_finder = interface_finder,
        .out = out,
        .err = err,
    };

    app::CommandDispatcher command_dispatcher;
    auto command = parseResult 
        ? parseResult->command 
        : &cli::HELP_CMD;
    auto options = parseResult 
        ? parseResult->options 
        : std::unordered_map<const cli::Option*, std::string>{};
    
    
    auto result = command_dispatcher.dispatch(
        command, 
        options, 
        executor_context
    );
    return result;
}