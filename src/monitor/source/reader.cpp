#include "monitor/reader.h"
#include <algorithm>
#include <cctype>
#include <cstddef>

#include <istream>
#include <nlohmann/json_fwd.hpp>
#include <optional>

#include <string>
#include <vector>

using monitor::Reader;

std::optional<nlohmann::json> Reader::read(std::istream& from) noexcept {
    std::string command_str;
    if (!std::getline(from, command_str, ';'))
        return std::nullopt;
    return parse_command(command_str);
}

std::vector<std::string> Reader::split(const std::string& src, const std::string& delims) noexcept {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;

    while ((end = src.find_first_of(delims, start)) != std::string::npos) {
        if (end > start) {
            tokens.emplace_back(src.substr(start, end - start));
        }
        start = end + 1;
    }

    if (start < src.size()) {
        tokens.emplace_back(src.substr(start));
    }

    return tokens;
}

std::pair<std::string, std::string> Reader::split_opt(const std::string& opt_token) {
    auto parts = split(opt_token, "/:");
    if (parts.empty()) {
        return {"", ""};
    } else if (parts.size() == 1) {
        return {parts[0], ""};
    }
    return {parts[0], parts[1]};
}

nlohmann::json Reader::make_options(const std::vector<std::string>& opt_src) noexcept {
    nlohmann::json options = R"({})"_json;
    for (auto& opt : opt_src) {
        auto [key, value] = split_opt(opt);
        if (key.empty()) {
            continue;
        }

        if (value.empty()) {
            options[key] = true;
        } else if (std::all_of(value.begin(), value.end(), ::isdigit)) {
            int casted = std::stoi(value);
            options[key] = casted;
        } else {
            options[key] = value;
        }
    }
    return options;
}

std::optional<nlohmann::json> Reader::parse_command(const std::string& source) noexcept {
    nlohmann::json parsed_command;

    auto tokens = split(source, " ;");
    if (tokens.empty())
        return std::nullopt;

    auto command_name_token = tokens[0];

    std::vector<std::string> data_tokens{};
    std::vector<std::string> opt_tokens{};

    for (std::size_t i = 1; i < tokens.size(); ++i) {
        auto token = tokens[i];
        if (token.starts_with('/')) {
            opt_tokens.push_back(token);
        } else {
            data_tokens.push_back(token);
        }
    }

    parsed_command["name"] = command_name_token;
    parsed_command["data"] = data_tokens;
    parsed_command["options"] = make_options(opt_tokens);

    return parsed_command;
}