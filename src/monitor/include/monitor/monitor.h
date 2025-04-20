#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include "monitor/logger_manager.h"

#include <config/monitor_config.h>

namespace monitor {
class Monitor final {
   public:
    Monitor();

    [[nodiscard]] std::optional<nlohmann::json> poll_command();
    void display(const std::string& message);

   private:
    std::string last_command_{};

    LoggerManager l_manager_{};
};
}  // namespace monitor