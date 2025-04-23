#pragma once

#include <string>
namespace monitor::utils {
class IDGenerator final {
   public:
    [[nodiscard]] static std::string generate() noexcept;
};
}  // namespace monitor::utils