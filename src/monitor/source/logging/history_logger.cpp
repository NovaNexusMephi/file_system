#include "monitor/logging/history_logger.hpp"
#include "spdlog/sinks/basic_file_sink.h"
using monitor::logging::HistoryLogger;

HistoryLogger::HistoryLogger(const fs::path& session_path) {
    auto file_path = session_path / "history.log";
    logger_ = spdlog::basic_logger_mt("history", file_path);
    logger_->set_pattern("[%Y-%m-%dT%H:%M:%S.%f] %v");
}

void HistoryLogger::log_start(const std::string& command_id, const std::string& command) {
    logger_->info("START | ID {} | CMD {}", command_id, command);
}

void HistoryLogger::log_commit(const std::string& command_id) {
    logger_->info("COMMIT | ID {}", command_id);
}

void HistoryLogger::log_rollback(const std::string& command_id, const std::string& error) {
    logger_->info("ROLLBACK | ID {} | ERROR {}", command_id, error);
}