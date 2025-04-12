#pragma once

#include <nlohmann/json_fwd.hpp>
#include "config/monitor_config.h"
#include "monitor/reader.h"
#include "monitor/writer.h"

namespace monitor {
class Monitor final {
   public:
    Monitor() = delete;
    explicit Monitor(config::MonitorConfig& config);

    [[nodiscard]] nlohmann::json read_command();

    void write_result(int status, std::string message);

   private:
    Reader reader_;
    Writer writer_;
};
}  // namespace monitor