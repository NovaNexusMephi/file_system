#include "monitor/monitor.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include "monitor/parser.hpp"
using monitor::Monitor;

Monitor::Monitor(const std::string& prompt, const std::string& logs) {
    std::filesystem::path s_dir = create_session_dir(logs);
    create_log_files(s_dir);

    prompt_ = prompt;
    logs_ = logs;
}

Monitor::~Monitor() {
    results_.close();
    history_.close();
}

std::optional<nlohmann::json> Monitor::get_command() noexcept {
    if (std::getline(std::cin, last_command_, ';')) {
        return Parser::parse(last_command_);
    }
    return std::nullopt;
}

void Monitor::write(const std::string& msg) {
    std::cout << msg << "\n";
    results_ << msg << "\n";
    history_ << last_command_ << ";";
}

std::string Monitor::generate_timestamp() noexcept {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm time_info = *std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::put_time(&time_info, "%Y-%m-%d_%H-%M-%S");
    oss << '.' << std::setw(3) << std::setfill('0') << ms.count();

    return oss.str();
}

std::filesystem::path Monitor::create_session_dir(const std::filesystem::path& d_path) {
    auto timestamp = generate_timestamp();
    std::filesystem::path session_dir = d_path / timestamp;
    std::filesystem::create_directories(session_dir);
    return session_dir;
}

void Monitor::create_log_files(const std::filesystem::path& s_dir) {
    std::filesystem::path res_log = s_dir / "results.log";
    results_.open(res_log);

    std::filesystem::path history_log = s_dir / "history.log";
    history_.open(history_log);
}