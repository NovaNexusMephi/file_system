#ifndef CONFIG_SYSTEM_INFO_CONFIG_H
#define CONFIG_SYSTEM_INFO_CONFIG_H

#include <libconfig.h++>
#include <string>
#include <vector>

namespace config {
struct SystemInfoConfig final {
    std::string volume_id;
    std::string owner_name;
    std::string system_name;
    std::vector<int> defect_table;

    SystemInfoConfig() = default;
    SystemInfoConfig(const libconfig::Setting& setting);
};
}  // namespace config

#endif