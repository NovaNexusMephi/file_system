#include "commands/free_command.hpp"

#include "commands/commands_constants.hpp"

std::string FreeCommand::execute() {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }

    std::string response = OK + ":\n";
    size_t block_number = 0;
    size_t index = 1;
    size_t used_space = 0;
    auto& catalog = receiver_.get_catalog();
    for (const auto& segment : catalog.get_segments()) {
        for (const auto& record : segment.get_records()) {
            if (record.get_type() == filesystem::FileType::FREE) {
                response += std::format("{}. {} {}\n", index, block_number + 1, record.get_size());
                index++;
            } else if (record.get_type() == filesystem::FileType::PERMANENT) {
                used_space += record.get_size();
            }
            block_number += record.get_size();
        }
    }
    if (catalog.get_free_direct_space() > 0) {
        block_number = catalog.get_free_direct_space() > 0 ? block_number : 0;
        response += std::format("{}. {} {}\n", index, block_number + 1, catalog.get_free_direct_space());
    }
    response += std::format("Occupied: {}\n", used_space);
    response += std::format("Free: {}\n", catalog.get_free_space());
    return response;
}
