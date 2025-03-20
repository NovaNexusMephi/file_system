#ifndef CONFIG_LOGGING_CONFIG_H
#define CONFIG_LOGGING_CONFIG_H

#include <string>
namespace config {

struct LoggingConfig final {
    struct Settings final {
        bool enabled;
        std::string level;
        std::string pattern;
    };

    Settings console;
    Settings file;
};
}  // namespace config

#endif