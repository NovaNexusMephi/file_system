#include "monitor/logging/log_manager.hpp"
#include <memory>
#include "monitor/logging/history_logger.hpp"
#include "monitor/logging/result_logger.hpp"
using monitor::logging::HistoryLogger;
using monitor::logging::LogManager;
using monitor::logging::ResultLogger;

LogManager::LogManager() {
    create_session_directory();
    history_logger_ = std::make_shared<HistoryLogger>(session_path_);
    result_logger_ = std::make_shared<ResultLogger>(session_path_);
}

fs::path LogManager::get_session_path() const noexcept {
    return session_path_;
}

const std::shared_ptr<HistoryLogger>& LogManager::get_history_logger() const {
    return history_logger_;
}

const std::shared_ptr<ResultLogger>& LogManager::get_result_logger() const {
    return result_logger_;
}

void LogManager::create_session_directory() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << "session_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << "_" << std::setfill('0') << std::setw(3) << ms.count();

    session_path_ = fs::path("logs") / oss.str();
    fs::create_directories(session_path_);
}