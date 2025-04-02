#ifndef CONFIG_SYSTEM_INFO_CONFIG_H
#define CONFIG_SYSTEM_INFO_CONFIG_H

#include <libconfig.h++>
#include <string>

namespace config {
/// @brief Holds configuration data for system information, including volume, owner, and system names.
struct SystemInfoConfig final {
   public:
    /// Name of the volume (e.g., "primary_volume").
    std::string volume_name{};
    /// Name of the owner (e.g., "admin").
    std::string owner_name{};
    /// Name of the system (e.g., "prod_server").
    std::string system_name{};

    /// @brief Default constructor. Initializes all fields to empty strings.
    SystemInfoConfig() = default;

    SystemInfoConfig(const libconfig::Setting& setting);
};
}  // namespace config

#endif