#pragma once

#include <nlohmann/json_fwd.hpp>
#include <optional>
namespace monitor::reading {
class InputManager final {
   public:
    InputManager() = default;
    std::optional<nlohmann::json> read_command();
};
}  // namespace monitor::reading