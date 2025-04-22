#pragma once

#include <filesystem>
#include <memory>
#include "spdlog/logger.h"
namespace fs = std::filesystem;

namespace monitor::logging {
class ResultLogger final {
   public:
    explicit ResultLogger(const fs::path& session_path);

    void log_success(const std::string& command_id, const std::string& result);

    void log_error(const std::string& command_id, const std::string& error);

   private:
    std::shared_ptr<spdlog::logger> logger_;
};
}  // namespace monitor::logging