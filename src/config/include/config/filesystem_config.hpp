#pragma once

#include <libconfig.h++>
#include <string>

namespace config {
/**
 * @brief Represents the configuration for system information.
 * 
 * This structure encapsulates details about the system, such as the volume ID, owner name, 
 * and system name. These fields are used to identify and describe the system in the context 
 * of the application's configuration.
 */
struct SystemInfoConfig final {
    std::string volume_id{};    ///< The unique identifier for the system's storage volume.
    std::string owner_name{};   ///< The name of the system's owner or administrator.
    std::string system_name{};  ///< The name or identifier of the system itself.
};

/**
 * @brief Represents the configuration for the filesystem.
 * 
 * This structure encapsulates the filesystem-related settings, including system information 
 * such as volume ID, owner name, and system name. It aggregates an instance of `SystemInfoConfig` 
 * to organize these details.
 */
struct FilesystemConfig final {
    SystemInfoConfig sys_info{};  ///< The system information configuration.

    /**
     * @brief Default constructor for FilesystemConfig.
     * 
     * Initializes the `sys_info` field with its default constructor.
     */
    FilesystemConfig() = default;

    /**
     * @brief Construct a new Filesystem Config object from a libconfig::Setting.
     * 
     * Initializes the FilesystemConfig object by reading values from the provided `settings`. 
     * The `sys_info` fields are populated based on the corresponding keys in the configuration.
     * 
     * @param[in] settings The libconfig::Setting object containing the filesystem configuration.
     */
    FilesystemConfig(const libconfig::Setting& settings);
};
}  // namespace config
