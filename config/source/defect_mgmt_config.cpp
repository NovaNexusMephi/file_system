#include "config/defect_mgmt_config.h"

#include <libconfig.h++>

#include "config/field_lookup.h"
using config::DefectMgmtConfig;

#define REPLACEMENT_BLOCKS_FIELD "replacement_blocks"
#define AUTO_MAP_FIELD "auto_map"

DefectMgmtConfig::DefectMgmtConfig(const libconfig::Setting& setting) {
  lookup_vector_field(setting, REPLACEMENT_BLOCKS_FIELD, replacement_blocks);
  lookup_scalar_field(setting, AUTO_MAP_FIELD, auto_remap);
}