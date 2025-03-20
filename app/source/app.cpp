#include "app/app.h"

#include <csignal>
#include <filesystem>

#include "config/config_manager.h"
using app::App;

App::App(const std::filesystem::path& cfg_path) : cfg_manager_(cfg_path) {}

void App::run() {
    start_up();

    std::signal(SIGINT, [](int signal) { signal_handler(signal); });
    std::signal(SIGTERM, [](int signal) { signal_handler(signal); });

    while (working_) {
        // TODO: implement main loop of application
    }

    shutdown();
}

void App::start_up() {
    // TODO: perform initialization tasks here
}

void App::shutdown() {
    // TODO: perform cleanup tasks here
}