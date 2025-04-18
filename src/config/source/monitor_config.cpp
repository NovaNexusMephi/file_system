#include "config/monitor_config.h"
#include "config/lookup.h"
#include <libconfig.h++>
using config::MonitorConfig;

constexpr std::string PROMPT_FIELD = "prompt";

MonitorConfig::MonitorConfig(const libconfig::Setting &settings) {
#ifdef FORCE_CONFIG
  auto validation = [](const std::string &str) -> bool { return !str.empty(); };
  prompt = validated_scalar<std::string>(settings, PROMPT_FIELD, validation);
#else
  prompt = scalar_of_default(settings, PROMPT_FIELD, "[enter]");
#endif
}