#include "monitor/reader.h"
#include <ios>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

using monitor::Reader;

std::optional<nlohmann::json> Reader::read() {
    nlohmann::json result;

    std::string command;
    if (!std::getline(stream_, command, ';'))
        return std::nullopt;

    std::istringstream ss(command);

    std::string name;
    ss >> name;
    result["command_name"] = name;

    nlohmann::json options{};
    std::string token{};
    while (ss >> token) {
        auto [key, value] = split_val(token);
        if (key.empty())
            continue;

        if (!value.has_value() || value->empty()) {
            options[key] = true;
        } else {
            try {
                size_t pos;
                int ival = std::stoi(*value, &pos);

                if (pos == value->size()) {
                    options[key] = ival;
                } else {
                    options[key] = *value;
                }
            } catch (const std::invalid_argument&) {
                options[key] = *value;
            } catch (const std::out_of_range&) {
                options[key] = *value;
            }
        }
    }

    result["options"] = options;
    return result;
}

std::string Reader::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

std::pair<std::string, std::optional<std::string>> Reader::split_val(const std::string& input) {
    if (input.empty() || input.front() != '\\') {
        return {"", std::nullopt};
    }

    std::string trimmed = trim(input.substr(1));

    size_t colon_pos = trimmed.find(':');
    if (colon_pos != std::string::npos) {
        std::string option = trim(trimmed.substr(0, colon_pos));
        std::string value = trim(trimmed.substr(colon_pos + 1));
        return {option, value.empty() ? std::nullopt : std::make_optional(value)};
    } else {
        return {trim(trimmed), std::nullopt};
    }
}