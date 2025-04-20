#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include <ostream>
#include "monitor/logger_manager.h"

#include <config/monitor_config.h>

namespace monitor {
class Monitor final {
   public:
    Monitor();
    ~Monitor();

    [[nodiscard]] std::optional<nlohmann::json> poll_command();
    void display(const std::string& message);

   private:
    std::string get_current_time();
    void write(std::ostream& stream, const std::string& message);

    std::string last_command_{};

    LoggerManager l_manager_{};
};
}  // namespace monitor