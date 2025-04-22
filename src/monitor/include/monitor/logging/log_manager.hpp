#pragma once

#include <filesystem>
#include <memory>
#include "monitor/logging/history_logger.hpp"
#include "monitor/logging/result_logger.hpp"
namespace fs = std::filesystem;

namespace monitor::logging {
class LogManager final {
   public:
    LogManager();

    [[nodiscard]] fs::path get_session_path() const noexcept;

    [[nodiscard]] const std::shared_ptr<HistoryLogger>& get_history_logger() const;

    [[nodiscard]] const std::shared_ptr<ResultLogger>& get_result_logger() const;

   private:
    void create_session_directory();

    fs::path session_path_{};
    std::shared_ptr<HistoryLogger> history_logger_{nullptr};
    std::shared_ptr<ResultLogger> result_logger_{nullptr};
};
}  // namespace monitor::logging