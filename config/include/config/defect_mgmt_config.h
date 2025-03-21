#ifndef CONFIG_DEFECT_MGMT_CONFIG_H
#define CONFIG_DEFECT_MGMT_CONFIG_H

#include <libconfig.h++>
#include <vector>

namespace config {
struct DefectMgmtConfig final {
  std::vector<int> replacement_blocks{};
  bool auto_remap = true;

  DefectMgmtConfig() = default;
  DefectMgmtConfig(const libconfig::Setting& setting);
};
}  // namespace config

#endif