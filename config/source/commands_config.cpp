#include "config/commands_config.h"

#include <libconfig.h++>
#include <stdexcept>

#include "config/constants.h"
#include "config/field_lookup.h"
using config::CommandsConfig;

#define INIT_COMMAND_GROUP "init"
#define INITIAL_SEGMENTS_FIELD "initial_segments"
#define PROMPT_VOLUME_INFO_FIELD "prompt_volume_info"

#define DIR_COMMAND_GROUP "dir"
#define SHOW_FREE_AREAS_FIELD "show_free_areas"
#define OUTPUT_TO_FILE_FIELD "output_to_file"
#define SHOW_VOLUME_INFO_FIELD "show_volume_info"

#define SQUEEZE_COMMAND_GROUP "squeeze"
#define DEFRAGMENT_FREE_SPACE_FIELD "defragment_free_space"
#define VERIFY_AFTER_FIELD "verify_after"

CommandsConfig::CommandsConfig(const libconfig::Setting& setting) {
    const libconfig::Setting& init_setting = setting[INIT_COMMAND_GROUP];
    lookup_scalar_field(init_setting, INITIAL_SEGMENTS_FIELD, init.initial_segments);
    lookup_scalar_field(init_setting, PROMPT_VOLUME_INFO_FIELD, init.prompt_volume_info);

    if (init.initial_segments != INITIAL_SEGMENT) {
        throw std::runtime_error(
            "invalid commands configuration: initial_segment must be equal to "
            "INITIAL_SEGMENT");
    }

    const libconfig::Setting& dir_setting = setting[DIR_COMMAND_GROUP];
    dir_setting.lookupValue(SHOW_FREE_AREAS_FIELD, dir.show_free_areas);
    lookup_scalar_field(dir_setting, SHOW_FREE_AREAS_FIELD, dir.show_free_areas);
    lookup_scalar_field(dir_setting, OUTPUT_TO_FILE_FIELD, dir.output_to_file);
    lookup_scalar_field(dir_setting, SHOW_VOLUME_INFO_FIELD, dir.show_volume_info);

    const libconfig::Setting& squeeze_setting = setting[SQUEEZE_COMMAND_GROUP];
    lookup_scalar_field(squeeze_setting, DEFRAGMENT_FREE_SPACE_FIELD, squeeze.defragment_free_space);
    lookup_scalar_field(squeeze_setting, VERIFY_AFTER_FIELD, squeeze.verify_after);
}