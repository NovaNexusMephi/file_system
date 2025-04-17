#pragma once

#include <libconfig.h++>
#include <string>
namespace config {

struct MonitorConfig final {
    std::string prompt;
    std::string history;
    std::string results;

    MonitorConfig() = default;
    MonitorConfig(const libconfig::Setting& settings);
};
}  // namespace config