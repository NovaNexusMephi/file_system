#ifndef _CONFIG_INFO_CONFIG_H_
#define _CONFIG_INFO_CONFIG_H_

#include <yaml-cpp/node/node.h>

#include <string>
namespace config {

/**
 * @brief This structure encapsulates metadata details such as the owner's name and the volume identifier
 *
 * Example YAML configuration:
 * @code{.yaml}
 * info:
 *   owner_name: "Alice"
 *   volume_id: "StorageVolume1"
 * @endcode
 */
struct InfoConfig final {
  /**
   * @brief Name of the owner of the system or volume.
   *
   * This value specifies the name of the individual or entity responsible for the volume.
   * If the configuration file does not provide this value or provides an invalid one,
   * the default value will be used.
   *
   * Default value: "Owner"
   */
  std::string owner_name = "Owner";

  /**
   * @brief Identifier for the system or volume.
   *
   * This value uniquely identifies the system or storage volume.
   * If the configuration file does not provide this value or provides an invalid one,
   * the default value will be used.
   *
   * Default value: "Volume"
   */
  std::string volume_id = "Volume";
};

/**
 * @brief Reads metadata information from a YAML node into an `InfoConfig` structure.
 *
 * This operator overload extracts the metadata details (owner name and volume ID) from the provided YAML node
 * and writes them into the specified `InfoConfig` structure.
 *
 * The YAML configuration file may contain the following keys under the `info` node:
 * - `owner_name`: A string representing the name of the owner.
 * - `volume_id`: A string representing the unique identifier of the system or volume.
 *
 *
 * @param[in] node A YAML node containing the metadata information. Missing or invalid fields will result in default
 * values being used.
 * @param[in,out] info_cfg A reference to the `InfoConfig` structure where the parsed metadata will be stored.
 */
inline void operator>>(const YAML::Node& node, InfoConfig& info_cfg) {
  if (node["owner_name"] && node["owner_name"].IsScalar()) {
    info_cfg.owner_name = node["owner_name"].as<std::string>();
  }
  if (node["volume_id"] && node["volume_id"].IsScalar()) {
    info_cfg.volume_id = node["volume_id"].as<std::string>();
  }
}

}  // namespace config

#endif