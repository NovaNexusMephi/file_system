#pragma once

#include <iostream>
#include <istream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>

namespace monitor {

class Reader final {
   public:
    Reader() = default;
    explicit Reader(const std::string& prompt) : prompt_(prompt){};
    [[nodiscard]] std::optional<nlohmann::json> read(std::istream& from = std::cin);

   private:
    std::string prompt_;
};
}  // namespace monitor