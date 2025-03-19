#include "config/info_config.h"

using namespace config;

void operator>>(const YAML::Node& node, InfoConfig& info_cfg) {
  if (node["owner_name"] && node["owner_name"].IsScalar())
    info_cfg.owner_name = node["owner_name"].as<std::string>();
  if (node["volume_id"] && node["volume_id"].IsScalar())
    info_cfg.volume_id = node["volume_id"].as<std::string>();
}
