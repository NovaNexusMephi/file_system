#include "config/filesystem_config.h"
#include <stdexcept>
#include "config/lookup.h"
using config::FilesystemConfig;

FilesystemConfig::FilesystemConfig(const libconfig::Setting& settings) {
    if (!settings.exists("system_info")) {
        throw std::runtime_error("Missing required section: system_info");
    }
    const libconfig::Setting& sys_info_setting = settings.lookup("system_info");

    sys_info.volume_id = get_required<std::string>(sys_info_setting, "volume_id");
    sys_info.owner_name = get_required<std::string>(sys_info_setting, "owner_name");
    sys_info.system_name = get_required<std::string>(sys_info_setting, "system_name");
}