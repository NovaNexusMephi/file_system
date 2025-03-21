#include "config/filesystem_config.h"

#include <filesystem>
#include <libconfig.h++>
#include <stdexcept>
using config::FileSystemConfig;

FileSystemConfig::FileSystemConfig(const std::filesystem::path& cfg_path) {
    if (cfg_path.empty()) {
        throw std::runtime_error("invalid path to configuration: empty");
    }
    libconfig::Config cfg;
    cfg.readFile(cfg_path);
    system_info = cfg.lookup("system_info");
    catalog = cfg.lookup("catalog");
    file_mgmt = cfg.lookup("file_mgmt");
    commands = cfg.lookup("commands");
    defect_mgmt = cfg.lookup("defect_mgmt");
    logging = cfg.lookup("logging");
}