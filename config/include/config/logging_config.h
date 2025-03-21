#ifndef CONFIG_LOGGING_CONFIG_H
#define CONFIG_LOGGING_CONFIG_H

#include <spdlog/spdlog.h>

#include <libconfig.h++>
#include <string>

namespace config {
struct LoggingConfig final {
  bool enable_file_logging = true;
  std::string log_pattern = "[%Y-%m-%d %H:%M:%S] [%l] %v";
  spdlog::level::level_enum level = spdlog::level::info;

  LoggingConfig() = default;
  explicit LoggingConfig(const libconfig::Setting& setting);
};
}  // namespace config

#endif