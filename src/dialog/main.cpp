#include <iostream>

#include "command_builder/command_builder.hpp"
#include "commands/exit_command.hpp"
#include "config/config.hpp"
#include "filesystem/filesystem.hpp"
#include "monitor/monitor.hpp"

int main(int argc, char* argv[]) {
    try {
        config::Config cfg = config::Config::prepare(argc, argv);
        monitor::Monitor monitor{cfg.prompt, cfg.logs};
        filesystem::FileSystem filesystem;
        CommandBuilder builder(filesystem);

        while (!std::cin.eof()) {
            auto precommand = monitor.get_command();
            if (!precommand.has_value()) {
                std::cout << "invalid input, use help to get list of commands" << std::endl;
                continue;
            }
            auto command = builder.build(precommand.value());
            auto msg = command->execute();
            monitor.write(msg);
        }

    } catch (const ExitException& exit) {
        std::cerr << exit.what() << "\n";
        return 1;
    } catch (const std::exception& fatal_error) {
        std::cerr << fatal_error.what() << "\n";
        return 1;
    }
}
