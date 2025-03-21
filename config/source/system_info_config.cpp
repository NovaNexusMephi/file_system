#include "config/system_info_config.h"

#include <libconfig.h++>

#include "config/field_lookup.h"
using config::SystemInfoConfig;

#define VOLUME_ID_FIELD "volume_id"
#define OWNER_NAME_FILED "owner_name"
#define SYSTEM_NAME_FILED "system_name"
#define DEFECT_TABLE_FILED "defect_table"

SystemInfoConfig::SystemInfoConfig(const libconfig::Setting& setting) {
  lookup_scalar_field(setting, VOLUME_ID_FIELD, volume_id);
  lookup_scalar_field(setting, OWNER_NAME_FILED, owner_name);
  lookup_scalar_field(setting, SYSTEM_NAME_FILED, system_name);
  lookup_scalar_field(setting, DEFECT_TABLE_FILED, defect_table);
}