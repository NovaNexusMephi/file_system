#include "config/file_mgmt_config.h"
#include <libconfig.h++>
#include "config/field_lookup.h"
using config::FileMgmtConfig;

#define ALLOW_TEMPORARY_FILES_FIElD "allow_temporary_files"
#define AUTO_CONVERT_TEMP_FIElD "auto_convert_temp"
#define PROTECT_DELETION_FIElD "protect_deletion"

FileMgmtConfig::FileMgmtConfig(const libconfig::Setting& setting) {
    lookup_scalar_field(setting, ALLOW_TEMPORARY_FILES_FIElD, allow_temporary_files);
    lookup_scalar_field(setting, AUTO_CONVERT_TEMP_FIElD, auto_convert_temp);
    lookup_scalar_field(setting, PROTECT_DELETION_FIElD, protect_deletion);
}