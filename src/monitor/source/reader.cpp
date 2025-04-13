#include "monitor/reader.h"
#include <istream>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include "monitor/command_parsing.h"
using monitor::Reader;

Reader::Reader(std::istream& stream, const std::string& prompt) {
    set_prompt(prompt);
    set_stream(stream);
}

std::string Reader::get_prompt() const noexcept {
    return prompt_;
}

void Reader::set_prompt(const std::string& new_prompt) {
    prompt_ = new_prompt;
}

void Reader::set_stream(std::istream& stream) & {
    stream_ = &stream;
}

std::optional<nlohmann::json> Reader::read() {
    std::string command_line;
    if (!std::getline(*stream_, command_line, ';'))
        return std::nullopt;
    return parse_command(command_line);
}