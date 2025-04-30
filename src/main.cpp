#include <exception>
#include <iostream>

#include <config/config.hpp>
using config::Config;

#include <monitor/monitor.hpp>
using monitor::Monitor;

#include <nlohmann/json.hpp>

#include <monitor/monitor.hpp>
using monitor::Monitor;

#include <nlohmann/json.hpp>

int main(int argc, char* argv[]) {
    try {
        Config cfg = Config::prepare(argc, argv);

        Monitor monitor{cfg.prompt, cfg.logs};

        while (!std::cin.eof()) {
            auto precommand = monitor.get_command();
            auto msg = (precommand.has_value() ? precommand->dump() : "no command gotten");
            if (msg == "no command gotten") {
                break;
            }
            monitor.write(msg);
        }

    } catch (const std::exception& fatal_error) {
        std::cerr << fatal_error.what() << "\n";
        return 1;
    }
}
