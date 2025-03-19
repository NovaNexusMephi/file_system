#include "config/fat_config.h"

using namespace config;

void operator>>(const YAML::Node& node, FATConfig& fat_config) {
  if (node["segment_number"] && node["segment_number"].IsScalar())
    fat_config.segment_number = node["segment_number"].as<std::uint8_t>();
}
