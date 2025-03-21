#include "config/logging_config.h"
#include <libconfig.h++>
#include "config/field_lookup.h"
using config::LoggingConfig;

#define ENABLE_FILE_LOGGING_FIELD "enable_file_logging_field"
#define LOG_PATTERN_FIELD "log_pattern"
#define LEVEL_FIELD "level"

LoggingConfig::LoggingConfig(const libconfig::Setting& setting) {
    lookup_scalar_field(setting, ENABLE_FILE_LOGGING_FIELD, enable_file_logging);
    lookup_scalar_field(setting, LOG_PATTERN_FIELD, log_pattern);
    lookup_scalar_field(setting, LEVEL_FIELD, level);
}