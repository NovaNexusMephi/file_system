#pragma once

#include <memory>
#include "spdlog/logger.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace monitor::logging {
class HistoryLogger final {
   public:
    explicit HistoryLogger(const fs::path& session_path);

    void log_start(const std::string& command_id, const std::string& command);

    void log_commit(const std::string& command_id);

    void log_rollback(const std::string& command_id, const std::string& error);

   private:
    std::shared_ptr<spdlog::logger> logger_;
};
}  // namespace monitor::logging