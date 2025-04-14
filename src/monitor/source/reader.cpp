#include "monitor/reader.h"
#include <cstdio>
#include <iostream>
#include <istream>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include "monitor/command_parsing.h"
using monitor::Reader;

std::optional<nlohmann::json> Reader::read(std::istream& from) {
    if (!from || !from.good()) {
        return std::nullopt;
    }

    std::cout << prompt_;

    std::string command_line;
    if (!(std::getline(from, command_line, ';'))) {
        return std::nullopt;
    }

    return parse_command(command_line);
}
