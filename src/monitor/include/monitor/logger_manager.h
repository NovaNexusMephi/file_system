#pragma once

#include <filesystem>
namespace fs = std::filesystem;

#include <memory>

#include <spdlog/spdlog.h>

namespace monitor {
class LoggerManager final {
   public:
    LoggerManager() = default;
    explicit LoggerManager(const fs::path& logs_dir);

    [[nodiscard]] std::shared_ptr<spdlog::logger> get(const std::string& logger_name);
    void create_logger(const std::string& l_name, const fs::path& f_path);

   private:
    std::filesystem::path run_dir_{};
};
}  // namespace monitor