#include "app/app.h"

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <csignal>
#include <filesystem>
#include <memory>

#include "config/config_manager.h"
using app::App;

App::App(const std::filesystem::path& cfg_path) : cfg_manager_(cfg_path) {}

void App::run() {
    start_up();

    logger_->info("starting application");
    logger_->info("configuration");

    std::signal(SIGINT, [](int signal) { signal_handler(signal); });
    std::signal(SIGTERM, [](int signal) { signal_handler(signal); });

    while (working_) {
        // TODO: implement main loop of application
    }

    shutdown();
}

void App::start_up() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);  // Set log level for console

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/my_log.txt", true);
    file_sink->set_level(spdlog::level::info);  // Set log level for file

    logger_ = std::make_shared<spdlog::logger>("logger", spdlog::sinks_init_list{console_sink, file_sink});
    logger_->set_level(spdlog::level::debug);
}

void App::shutdown() {
    // TODO: perform cleanup tasks here
}

void App::signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
    }
}