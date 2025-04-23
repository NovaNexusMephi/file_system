#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>
#include "monitor/logging/id_generator.hpp"
#include "monitor/logging/log_manager.hpp"
#include "monitor/reading/input_manager.hpp"

int main() {
    try {
        monitor::reading::InputManager input_manager{};
        monitor::logging::LogManager log_manager{};

        while (true) {
            auto precommand = input_manager.read_command();
            if (precommand.has_value()) {
                auto command_id = monitor::logging::IDGenerator::generate();
                log_manager.get_history_logger()->log_start(command_id, precommand->dump());
                log_manager.get_result_logger()->log_success(command_id, precommand->dump());
                log_manager.get_history_logger()->log_commit(command_id);
            } else {
                break;
            }
        }

    } catch (const std::exception& fatal_error) {
        std::cout << fatal_error.what() << std::endl;
        return 1;
    }
    return 0;
}