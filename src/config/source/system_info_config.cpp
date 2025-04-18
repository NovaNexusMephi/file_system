#include "config/system_info_config.h"
#include <libconfig.h++>
#include <string>
#include "config/lookup.h"
#include "lib/constants.h"
using config::SystemInfoConfig;

static const std::string VOLUME_NAME_FIELD = "volume_name";
static const std::string OWNER_NAME_FIELD = "owner_name";
static const std::string SYSTEM_NAME_FIELD = "system_name";

SystemInfoConfig::SystemInfoConfig(const libconfig::Setting& setting) {
#ifdef FORCE_CONFIG
    auto validation = [](const std::string& str) -> bool {
        return !str.empty();
    };
    volume_name = validated_scalar<std::string>(setting, VOLUME_NAME_FIELD, validation);
    owner_name = validated_scalar<std::string>(setting, OWNER_NAME_FIELD, validation);
    system_name = validated_scalar<std::string>(setting, SYSTEM_NAME_FIELD, validation);
#else
    volume_name = scalar_of_default<std::string>(setting, VOLUME_NAME_FIELD, lib::DEFAULT_VOLUME_NAME);
    owner_name = scalar_of_default<std::string>(setting, OWNER_NAME_FIELD, lib::DEFAULT_OWNER_NAME);
    system_name = scalar_of_default<std::string>(setting, SYSTEM_NAME_FIELD, lib::DEFAULT_SYSTEM_NAME);
#endif
}