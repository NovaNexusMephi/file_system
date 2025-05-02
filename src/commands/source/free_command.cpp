#include "commands/free_command.hpp"

#include "commands/commands_constants.hpp"

std::string FreeCommand::execute() {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }

    std::string response = OK + ":\n";
    size_t block_number = 0;
    size_t index = 0;
    auto& catalog = receiver_.get_catalog();
    for (const auto& segment : catalog.get_segments()) {
        for (const auto& record : segment.get_records()) {
            if (record.get_type() == filesystem::FileType::FREE) {
                response += std::format("{} {} {}\n", index, block_number, record.get_size());
            }
            block_number += record.get_size();
            index++;
        }
    }
    return response;
}
