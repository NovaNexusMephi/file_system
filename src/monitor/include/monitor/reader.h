#pragma once

#include <iostream>
#include <istream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>

namespace monitor {

class Reader final {
   public:
    Reader() = default;
    [[nodiscard]] static std::optional<nlohmann::json> read(std::istream& from = std::cin);
};
}  // namespace monitor