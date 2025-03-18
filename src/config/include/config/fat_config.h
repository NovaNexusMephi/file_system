#ifndef _CONFIG_FAT_CONFIG_H_
#define _CONFIG_FAT_CONFIG_H_

#include <yaml-cpp/node/node.h>

#include <cstdint>

namespace config {

/**
 * @brief Represents the configuration of a file allocation table (FAT).
 *
 * This structure encapsulates the configuration parameters required for managing
 * a file allocation table. It includes details such as the number of segments in the FAT.
 *
 * Example YAML configuration:
 * @code{.yaml}
 * fat:
 *   segment_number: 15
 * @endcode
 */
struct FATConfig final {
  /**
   * @brief Number of segments in the file allocation table.
   *
   * This value specifies the total number of segments that make up the file allocation table.
   * If the configuration file does not provide this value or provides an invalid one,
   * the default value will be used.
   *
   * Default value: 15
   */
  std::uint8_t segment_number = 15;
};

/**
 * @brief Reads the configuration of the file allocation table (FAT) from a YAML node.
 *
 * This operator overload extracts the configuration details of the file allocation table
 * from the provided YAML node and writes them into the specified `FATConfig` structure.
 *
 * The YAML configuration file may contain the following key:
 * - `segment_number`: An unsigned 8-bit integer representing the number of segments in the FAT.
 *
 * @param[in] node A YAML node containing the configuration of the file allocation table.
 *                 Missing or invalid fields will result in default values being used.
 * @param[in,out] fat_config A reference to the `FATConfig` structure where the parsed
 *                           configuration will be stored.
 */
inline void operator>>(const YAML::Node& node, FATConfig& fat_config) {
  if (node["segment_number"] && node["segment_number"].IsScalar()) {
    fat_config.segment_number = node["segment_number"].as<std::uint8_t>();
  }
}

}  // namespace config

#endif