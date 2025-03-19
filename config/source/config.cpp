#include "config/config.h"

#include <yaml-cpp/node/parse.h>

#include <filesystem>

using namespace config;

Config::Config(const std::filesystem::path &cfg_path) {
  if (std::filesystem::exists(cfg_path)) {
    auto cfg = YAML::LoadFile(cfg_path);
    cfg["fat"] >> fat_cfg_;
    cfg["info"] >> info_cfg_;
  }
}
