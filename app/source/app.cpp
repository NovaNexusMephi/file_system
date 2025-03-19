#include "app/app.h"
#include "config/config.h"
#include <filesystem>
#include <iostream>
using app::App;

App::App(const std::filesystem::path &cfg_path) {
  if (!std::filesystem::exists(cfg_path)) {
    // TODO: Implement searching in default placement
  }
  cfg_ = config::Config{cfg_path};
}

void App::run() {
  std::cout << "Hello there! We are running with following configuration:"
            << "\n";
  std::cout << "\tFAT segment number: " << cfg_.get_fat_config().segment_number
            << "\n";
  std::cout << "\tUser info block:\n";
  std::cout << "\t\towner name:" << cfg_.get_info_config().owner_name << "\n";
  std::cout << "\t\tvolume id:" << cfg_.get_info_config().volume_id << "\n";

  // TODO: Implement main application loop
}