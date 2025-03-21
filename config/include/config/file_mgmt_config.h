#ifndef CONFIG_FILE_MGMT_CONFIG_H
#define CONFIG_FILE_MGMT_CONFIG_H

#include <libconfig.h++>

namespace config {

struct FileMgmtConfig final {
    bool allow_temporary_files = true;
    bool auto_convert_temp = true;
    bool protect_deletion = false;

    FileMgmtConfig() = default;
    FileMgmtConfig(const libconfig::Setting& setting);
};
}  // namespace config

#endif