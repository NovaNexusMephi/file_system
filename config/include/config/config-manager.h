#ifndef CONFIG_CONFIG_H
#define CONFIG_CONFIG_H

#include <filesystem>

namespace config {
class ConfigManager final {
public:
  ConfigManager() = default;

  void read_cfg(const std::filesystem::path &cfg_path);

private:
};
} // namespace config

#endif