#ifndef CONFIG_FILESYSTEM_CONFIG_H
#define CONFIG_FILESYSTEM_CONFIG_H

#include "system_info_config.h"
#include <libconfig.h++>

namespace config {
struct FilesystemConfig final {
  SystemInfoConfig system_info_cfg{};
  int segments_number{};

  FilesystemConfig() = default;
  FilesystemConfig(const libconfig::Setting &settings);
};
} // namespace config

#endif