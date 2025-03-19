/// @file config_manager.h
/// @brief Configuration manager for filesystem and simulation monitor
/// @details Uses libconfig to parse structured configuration files and validate parameters

#ifndef CONFIG_CONFIG_MANAGER_H
#define CONFIG_CONFIG_MANAGER_H

#include <filesystem>
#include <libconfig.h++>

#include "config/fat_config.h"
#include "config/system_info_config.h"

namespace config {

/// @addtogroup configuration
/// @{

/// @class ConfigManager
/// @brief Central configuration management class for system parameters
/// @details Handles parsing and validation of configuration files using libconfig [[6]]
class ConfigManager final {
   public:
    /// @brief Constructor with configuration file path
    /// @param[in] cfg_path Path to configuration file (supports JSON-like format)
    /// @throws std::runtime_error on parse errors, I/O errors, or validation failures
    explicit ConfigManager(const std::filesystem::path& cfg_path);

    /// Retrieves configuration of file allocation table.
    [[nodiscard]] const FATConfig& get_fat_config() const noexcept;

    /// Retrieves configuration of system information block.
    [[nodiscard]] const SystemInfoConfig& get_system_info_config() const noexcept;

   private:
    /// @brief Parse FAT-specific configuration
    /// @param cfg libconfig object containing parsed file
    /// @throws std::out_of_range if segment number exceeds valid range
    void read_fat_config(const libconfig::Config& cfg);

    /// @brief Parse system information configuration
    /// @param cfg libconfig object containing parsed file
    void read_system_info_config(const libconfig::Config& cfg);

    FATConfig fat_cfg_{};                 ///< Validated FAT configuration parameters
    SystemInfoConfig system_info_cfg_{};  ///< Validated system information parameters
};

/// @}
}  // namespace config

#endif
