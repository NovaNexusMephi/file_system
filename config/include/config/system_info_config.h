#ifndef CONFIG_SYSTEM_INFO_CONFIG_H
#define CONFIG_SYSTEM_INFO_CONFIG_H

#include <string>
namespace config {

/// @addtogroup configuration
/// @{

/// @struct SystemInfoConfig
/// @brief Represents configuration of system information block.
struct SystemInfoConfig final {
    /// @brief Version structure for semantic versioning
    struct Version final {
        int major{}, minor{}, patch{};  ///< Version components

        Version() = default;
        Version(int major, int minor, int patch) : major(major), minor(minor), patch(patch) {}
    };

    Version version{};          ///< Filesystem version (MAJOR.MINOR.PATCH)
    std::string owner_name{};   ///< System owner identifier
    std::string volume_name{};  ///< Volume identifier
    std::string system_name{};  ///< Filesystem name
};

/// @}
}  // namespace config

#endif