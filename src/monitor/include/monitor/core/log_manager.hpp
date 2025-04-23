#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>
#include "monitor/core/log_wrapper.hpp"
namespace fs = std::filesystem;

namespace monitor::core {
class LogManager final {
   public:
    LogManager();

    [[nodiscard]] fs::path get_session_path() const noexcept;

    void register_wrapper(const std::string& name, LogWrapper& wrapper);
    [[nodiscard]] std::shared_ptr<LogWrapper> get_wrapper(const std::string& name) const noexcept;

   private:
    void create_session_directory();

    fs::path session_path_{};
    std::unordered_map<std::string, std::shared_ptr<LogWrapper>> wrappers_{
        {"history", nullptr},
        {"results", nullptr},
    };
};
}  // namespace monitor::core