#include "monitor/reading/input_manager.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include "monitor/reading/command_parsing.h"
using monitor::reading::InputManager;

std::optional<nlohmann::json> InputManager::read_command() {
    std::string command_line;
    if (!std::getline(std::cin, command_line, ':')) {
        return std::nullopt;
    }
    return parse_command(command_line);
}