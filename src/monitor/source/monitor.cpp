#include "monitor/monitor.h"
#include <ctime>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include "monitor/command_parsing.h"

using monitor::Monitor;

Monitor::Monitor() : l_manager_("logs") {
    l_manager_.create_logger("history", "history.log");
    l_manager_.create_logger("results", "results.log");
}

std::optional<nlohmann::json> Monitor::poll_command() {
    std::string command_line;
    if (!(std::getline(std::cin, command_line, ';'))) {
        return std::nullopt;
    }
    last_command_ = command_line + ";";

    return parse_command(last_command_);
}

void Monitor::display(const std::string& message) {
    l_manager_.get("history")->info(message);
    l_manager_.get("results")->info(message);
}