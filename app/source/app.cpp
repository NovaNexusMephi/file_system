#include "app/app.h"
#include <filesystem>
#include <iostream>
using app::App;

App::App(const std::filesystem::path& cfg_path) : cfg_(cfg_path) {}

void App::run() {
    std::cout << "Hello, there!" << "\n";
}