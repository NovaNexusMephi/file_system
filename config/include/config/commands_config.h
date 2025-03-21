#ifndef CONFIG_COMMANDS_CONFIG_H
#define CONFIG_COMMANDS_CONFIG_H

#include <libconfig.h++>
namespace config {
struct CommandsConfig final {
  struct InitConfig {
    int initial_segments = 1;
    bool prompt_volume_info = true;
  } init;

  struct DirConfig final {
    bool show_free_areas = false;
    bool output_to_file = false;
    bool show_volume_info = true;
  } dir;

  struct SqueezeConfig final {
    bool defragment_free_space = true;
    bool verify_after = false;
  } squeeze;

  CommandsConfig() = default;
  explicit CommandsConfig(const libconfig::Setting& setting);
};
}  // namespace config

#endif