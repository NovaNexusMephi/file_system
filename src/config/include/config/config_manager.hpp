#pragma once

#include <filesystem>
#include "config/filesystem_config.hpp"
#include "config/monitor_config.hpp"

namespace config {

/**
 * @brief Manages the loading and access of configuration settings for the application.
 * 
 * This class is responsible for reading configuration settings from a file, parsing them, 
 * and providing access to structured configuration objects such as `FilesystemConfig` and `MonitorConfig`. 
 * It ensures that the configuration file exists and handles errors during file reading or parsing.
 */
class ConfigManager final {
   public:
    /**
     * @brief Construct a new Config Manager object.
     * 
     * Initializes the ConfigManager by reading and parsing the configuration file located at `cfg_path`. 
     * The file is expected to contain structured settings for the filesystem and monitoring configurations. 
     * Throws an exception if the file does not exist, cannot be read, or contains invalid data.
     * 
     * @param cfg_path The path to the configuration file.
     * @throws std::runtime_error If the configuration file is missing, unreadable, or contains errors.
     */
    explicit ConfigManager(const std::filesystem::path& cfg_path);

    /**
     * @brief Get the filesystem configuration object.
     * 
     * Provides read-only access to the `FilesystemConfig` object loaded from the configuration file.
     * 
     * @return const FilesystemConfig& A reference to the filesystem configuration.
     */
    [[nodiscard]] const FilesystemConfig& get_fs_config() const noexcept;

    /**
     * @brief Get the monitor configuration object.
     * 
     * Provides read-only access to the `MonitorConfig` object loaded from the configuration file.
     * 
     * @return const MonitorConfig& A reference to the monitor configuration.
     */
    [[nodiscard]] const MonitorConfig& get_monitor_config() const noexcept;

   private:
    FilesystemConfig fs_config_{};
    MonitorConfig monitor_config_{};
};
}  // namespace config