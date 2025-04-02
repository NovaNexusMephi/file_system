#include "config/filesystem_config.h"
#include "config/lookup.h"
#include "lib/constants.h"
using config::FilesystemConfig;

constexpr std::string SYSTEM_INFO_GROUP = "system_info";
constexpr std::string SEGMENT_NUMBER_FIELD = "segment_number";

FilesystemConfig::FilesystemConfig(const libconfig::Setting& settings) {
    if (!settings.exists(SYSTEM_INFO_GROUP)) {
        throw std::runtime_error("invalid configuration: missing group " + SYSTEM_INFO_GROUP + ".");
    }
    system_info_cfg = settings[SYSTEM_INFO_GROUP];
#ifdef FORCE_CONFIG
    auto validation = [](const short& value) -> bool {
        return lib::MIN_SEGMENT_NUMBER <= value && value <= lib::MAX_SEGMENT_NUMBER;
    };
    segments_number = validated_scalar<int>(settings, SEGMENT_NUMBER_FIELD, validation);
#else
    segments_number = scalar_of_default<int>(settings, SEGMENT_NUMBER_FIELD, lib::DEFAULT_SEGMENT_NUMBER);
#endif
}