#ifndef CONFIG_FILE_MGMT_CONFIG_H
#define CONFIG_FILE_MGMT_CONFIG_H

#include <libconfig.h++>

namespace config {
enum class AllocationStrategy { BEST_FIT, LARGEST_FREE, HALF_OR_SECOND };

struct FileMgmtConfig final {
    AllocationStrategy allocation_strategy = AllocationStrategy::BEST_FIT;

    bool allow_temporary_files = true;
    bool auto_convert_temp = true;
    bool protect_deletion = false;

    FileMgmtConfig() = default;
    FileMgmtConfig(const libconfig::Setting& setting);
};
}  // namespace config

#endif