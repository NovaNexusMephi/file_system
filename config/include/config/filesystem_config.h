#ifndef CONFIG_FILESYSTEM_CONFIG_H
#define CONFIG_FILESYSTEM_CONFIG_H

#include <filesystem>
#include <libconfig.h++>

#include "config/catalog_config.h"
#include "config/commands_config.h"
#include "config/defect_mgmt_config.h"
#include "config/file_mgmt_config.h"
#include "config/logging_config.h"
#include "config/system_info_config.h"

namespace config {
struct FileSystemConfig {
    SystemInfoConfig system_info{};
    CatalogConfig catalog{};
    FileMgmtConfig file_mgmt{};
    CommandsConfig commands{};
    DefectMgmtConfig defect_mgmt{};
    LoggingConfig logging{};

    FileSystemConfig() = default;
    explicit FileSystemConfig(const std::filesystem::path& cfg_path);
};
}  // namespace config

#endif