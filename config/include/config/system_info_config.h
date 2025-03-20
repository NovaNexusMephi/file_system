#ifndef CONFIG_SYSTEM_INFO_CONFIG_H
#define CONFIG_SYSTEM_INFO_CONFIG_H

#include <cstdint>
#include <libconfig.h++>
#include <string>
#include <vector>

namespace config {
struct SystemInfoConfig final {
    std::string volume_id;
    std::string owner_name;
    std::string system_name;
    std::vector<uint16_t> defect_table;

    SystemInfoConfig() = default;
    explicit SystemInfoConfig(const libconfig::Setting& setting);
};
}  // namespace config

#endif