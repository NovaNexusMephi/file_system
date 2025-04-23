#include "monitor/logging/result_logger.hpp"
#include "spdlog/sinks/basic_file_sink.h"
using monitor::logging::ResultLogger;

ResultLogger::ResultLogger(const fs::path& session_path) {
    auto file_path = session_path / "result.log";
    logger_ = spdlog::basic_logger_mt("result", file_path.string());
    logger_->set_pattern("[%Y-%m-%dT%H:%M:%S.%f] %v");
}

void ResultLogger::log_success(const std::string& command_id, const std::string& result) {
    logger_->info("SUCCESS ID {} RESULT {}", command_id, result);
    logger_->flush();
}

void ResultLogger::log_error(const std::string& command_id, const std::string& error) {
    logger_->info("ERROR ID {} DETAILS {}", command_id, error);
    logger_->flush();
}