#include "config/monitor_config.h"
#include <libconfig.h++>
#include <string>
#include "config/lookup.h"
using config::MonitorConfig;

MonitorConfig::MonitorConfig(const libconfig::Setting& setting) {
    prompt = get_required<std::string>(setting, "prompt");
    history = get_required<std::string>(setting, "history");
    results = get_required<std::string>(setting, "results");
}