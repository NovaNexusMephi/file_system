#pragma once

#include <memory>
#include <string>
#include <utility>
#include "spdlog/logger.h"
namespace monitor::core {
class LogWrapper {
   public:
    LogWrapper() = default;
    virtual ~LogWrapper() = default;

   protected:
    template <typename... Args>
    void log(const std::string& format, Args&&... args) {
        logger_->info(format, std::forward<Args>(args)...);
    }

    std::shared_ptr<spdlog::logger> logger_{nullptr};
};
}  // namespace monitor::core