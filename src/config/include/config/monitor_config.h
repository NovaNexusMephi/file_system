#ifndef CONFIG_MONITOR_CONFIG_H
#define CONFIG_MONITOR_CONFIG_H

#include <libconfig.h++>
#include <string>

namespace config {
struct MonitorConfig final {
   public:
    std::string prompt{};

    MonitorConfig() = default;
    MonitorConfig(const libconfig::Setting& settings);
};
}  // namespace config

#endif