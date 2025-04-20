#include "monitor/monitor.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include "monitor/command_parsing.h"

using monitor::Monitor;

Monitor::Monitor() : l_manager_("logs") {
    l_manager_.create_logger("history", "history.log");
    l_manager_.create_logger("results", "results.log");
}

Monitor::~Monitor() {}

std::optional<nlohmann::json> Monitor::poll_command() {
    std::string command_line;
    if (!(std::getline(std::cin, command_line, ';'))) {
        return std::nullopt;
    }
    last_command_ = command_line + ";";

    return parse_command(last_command_);
}

void Monitor::write(std::ostream& stream, const std::string& msg) {
    stream << msg << std::endl;
    stream.flush();
}

std::string Monitor::get_current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&current_time), "%Y-%m-%d_%H:%M:%S");

    return oss.str();
}

void Monitor::display(const std::string& message) {
    l_manager_.get("history")->info(message);
    l_manager_.get("results")->info(message);
}