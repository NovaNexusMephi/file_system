#pragma once

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>
namespace monitor::utils {
class CommandParser final {
   public:
    [[nodiscard]] static std::optional<nlohmann::json> parse(const std::string& cmd) noexcept;

   private:
    [[nodiscard]] static std::vector<std::string> split(const std::string& str, const std::string& delims) noexcept;
    [[nodiscard]] static std::pair<std::string, std::string> parse_opt(const std::string& opt_token) noexcept;
    [[nodiscard]] static nlohmann::json make_opt_obj(const std::vector<std::string>& opt_tokens) noexcept;
    [[nodiscard]] static std::pair<std::string, std::vector<std::string>> make_argv(
        const std::vector<std::string>& arg_tokens) noexcept;
};
}  // namespace monitor::utils