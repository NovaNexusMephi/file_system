#include "commands/sort_command.hpp"

#include "commands/commands_constants.hpp"

#include <iostream>

auto get_extension = [](const std::string& line) -> std::string {
    size_t pos = line.find_last_of('.');
    return (pos == std::string::npos) ? "" : line.substr(pos);
};

auto extract_date = [](const std::string& line) -> std::string {
    size_t pos = line.rfind(' ');
    return (pos == std::string::npos) ? "" : line.substr(pos + 1);
};

auto extract_size = [](const std::string& line) -> size_t {
    size_t pos = line.find(" Blocks");
    if (pos == std::string::npos)
        return 0;
    std::string size_str = line.substr(0, pos);
    size_t space_pos = size_str.find_last_of(' ');
    return std::stoul(size_str.substr(space_pos + 1));
};

auto name_compare = [](const std::string& a, const std::string& b) -> bool {
    return a < b;
};

auto extension_compare = [](const std::string& a, const std::string& b) -> bool {
    return get_extension(a) < get_extension(b);
};

auto date_compare = [](const std::string& a, const std::string& b) -> bool {
    return extract_date(a) < extract_date(b);
};

auto size_compare = [](const std::string& a, const std::string& b) -> bool {
    return extract_size(a) < extract_size(b);
};

std::string SortCommand::execute() {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }
    std::vector<std::string> result;
    std::string temp;
    for (const auto& segment : receiver_.get_catalog().get_segments()) {
        for (const auto& record : segment.get_records()) {
            if (record.get_type() != filesystem::FileType::FREE && record.get_type() != filesystem::FileType::BLOCKED) {
                temp = std::format("{} {} Blocks {}", record.get_filename(), record.get_size(), record.get_timestamp());
            }
            result.push_back(temp);
        }
    }
    if (sort_by_ == "ext") {
        std::sort(result.begin(), result.end(), extension_compare);
    } else if (sort_by_ == "date") {
        std::sort(result.begin(), result.end(), date_compare);
    } else if (sort_by_ == "size") {
        std::sort(result.begin(), result.end(), size_compare);
    }
    else {
        std::sort(result.begin(), result.end(), name_compare);
    }
    if (inv_ == "inv") {
        std::reverse(result.begin(), result.end());
    }
    std::string response = OK + ":\n";
    for (const auto& line : result) {
        response += line + "\n";
    }
    return response;
}
