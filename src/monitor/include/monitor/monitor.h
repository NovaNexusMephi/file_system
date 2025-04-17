#pragma once

#include <filesystem>
#include "config/monitor_config.h"
#include "monitor/reader.h"
#include "monitor/writer.h"

namespace monitor {
class Monitor final {
   public:
    Monitor() = default;
    explicit Monitor(const config::MonitorConfig& cfg);

   private:
    Reader reader_;
    Writer writer_;

    std::filesystem::path history_file_;
    std::filesystem::path results_file_;
};
}  // namespace monitor