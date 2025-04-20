#include "monitor/logger_manager.h"
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog-inl.h>
#include <filesystem>
#include <memory>
using monitor::LoggerManager;

LoggerManager::LoggerManager(const fs::path& logs_dir) {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d_%H-%M-%S");
    std::string timestamp = ss.str();

    run_dir_ = logs_dir / timestamp;

    if (!fs::exists(logs_dir))
        fs::create_directory(logs_dir);
    fs::create_directory(run_dir_);
}

std::shared_ptr<spdlog::logger> LoggerManager::get(const std::string& l_name) {
    return spdlog::get(l_name);
}

void LoggerManager::create_logger(const std::string& l_name, const fs::path& f_path) {
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(run_dir_ / f_path, true);

    auto l = std::make_shared<spdlog::logger>(l_name, sink);
    l->set_pattern("%v");
    l->set_level(spdlog::level::trace);
    l->flush_on(spdlog::level::trace);

    spdlog::register_logger(l);
}