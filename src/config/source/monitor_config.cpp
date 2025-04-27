#include <libconfig.h++>
#include <string>

#include "config/lookup.hpp"

#include "config/monitor_config.hpp"
using config::InputConfig;
using config::MonitorConfig;
using config::OutputConfig;

const std::string default_prompt = "[default prompt]";
const bool default_wait_for_input = true;
const std::string default_from = "";

MonitorConfig::MonitorConfig(const libconfig::Setting& setting)
    : input(setting.lookup("input")), output(setting.lookup("output")) {}

InputConfig::InputConfig(const libconfig::Setting& setting)
    : prompt(get_default<std::string>(setting, "prompt", default_prompt)),
      wait_for_input(get_default<bool>(setting, "wait_for_input", default_wait_for_input)),
      from(get_default<std::string>(setting, "from", default_from)) {}

OutputConfig::OutputConfig(const libconfig::Setting& setting) : to(get_required<std::string>(setting, "to")) {}