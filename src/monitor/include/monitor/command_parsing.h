#pragma once

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace monitor {

[[nodiscard]] std::optional<nlohmann::json>
parse_command(const std::string &cmd_line) noexcept;

[[nodiscard]] std::vector<std::string>
split(const std::string &str, const std::string &delims) noexcept;

[[nodiscard]] std::pair<std::string, std::string>
make_opt(const std::string &opt_token) noexcept;

[[nodiscard]] nlohmann::json
make_opt_obj(const std::vector<std::string> &opt_tokens) noexcept;

[[nodiscard]] std::pair<std::string, std::vector<std::string>>
name_params(const std::vector<std::string> &arg_tokens) noexcept;

} // namespace monitor