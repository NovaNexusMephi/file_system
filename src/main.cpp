#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>
#include "monitor/monitor.h"

int main() {
    try {
        monitor::Monitor monitor{};
        while (true) {
            auto command_date = monitor.poll_command();
            monitor.display(command_date.has_value() ? command_date->dump() : "not a command");
            monitor.flush_history();
        }
    } catch (const std::exception& fatal_error) {
        std::cout << fatal_error.what() << std::endl;
        return 1;
    }
    return 0;
}