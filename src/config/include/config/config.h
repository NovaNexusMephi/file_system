#ifndef _CONFIG_CONFIG_H_
#define _CONFIG_CONFIG_H_

#include <filesystem>

#include "fat_config.h"
#include "info_config.h"

namespace config {

/**
 * @brief This class encapsulates the configuration details required for managing the application.
 *
 * It includes configurations for the file allocation table (FAT) and user-specified metadata information.
 * The configuration is typically loaded from a YAML file during initialization.
 *
 * @see FATConfig
 * @see InfoConfig
 */
class Config final {
 public:
  /**
   * @brief Constructs a new `Config` instance.
   *
   * This constructor initializes a `Config` object by reading and parsing the specified configuration file.
   * If no path is provided or file does not exists by gotten it, the configuration will be initialized with default
   * values.
   *
   * @param[in] cfg_path Path to the configuration file.
   *
   * @see FATConfig
   * @see InfoConfig
   */
  explicit Config(const std::filesystem::path& cfg_path = "");

  /**
   * @brief Retrieves the configuration of the file allocation table (FAT).
   *
   * This method provides access to the `FATConfig` structure, which contains details such as
   * the number of segments in the FAT. If the configuration file was invalid or missing,
   * the returned configuration will contain default values.
   *
   * @return A constant reference to the `FATConfig` structure representing the FAT configuration.
   *
   * @see FATConfig
   */
  [[nodiscard]] inline const FATConfig& get_fat_config() const noexcept { return fat_cfg_; }

  /**
   * @brief Retrieves the user-specified metadata information configuration.
   *
   * This method provides access to the `InfoConfig` structure, which contains metadata details such as
   * the owner's name and volume identifier. If the configuration file was invalid or missing,
   * the returned configuration will contain default values.
   *
   * @return A constant reference to the `InfoConfig` structure representing the metadata configuration.
   *
   * @see InfoConfig
   */
  [[nodiscard]] inline const InfoConfig& get_info_config() const noexcept { return info_cfg_; }

 private:
  FATConfig fat_cfg_;
  InfoConfig info_cfg_;
};
}  // namespace config

#endif