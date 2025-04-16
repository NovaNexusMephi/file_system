#pragma once

#include <libconfig.h++>
#include <string>
namespace config {

struct SystemInfoConfig final {
    std::string volume_id{};
    std::string owner_name{};
    std::string system_name{};
};

struct FilesystemConfig final {
    SystemInfoConfig sys_info{};

    FilesystemConfig() = default;
    FilesystemConfig(const libconfig::Setting& settings);
};
}  // namespace config
