#pragma once

#include <istream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>

namespace monitor {

class Reader final {
   public:
    Reader() = delete;
    Reader(std::istream& stream, const std::string& prompt);

    [[nodiscard]] std::string get_prompt() const noexcept;
    void set_prompt(const std::string& new_prompt);

    void set_stream(std::istream& new_stream) &;
    void set_stream(std::istream&&) = delete;

    [[nodiscard]] std::optional<nlohmann::json> read();

   private:
    std::string prompt_{};
    std::istream* stream_{nullptr};
};
}  // namespace monitor