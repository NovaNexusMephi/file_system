#include "commands/dir_command.hpp"

#include "filesystem/file_record.hpp"

std::string DirCommand::execute(const nlohmann::json& json) {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }
    auto flag = json["options"]["full"].get<bool>();
    std::string response = OK + " :\n", temp;
    if (flag) {
        response += ("Volume:" + receiver_.get_info().get_volume_name() +
                     ", Owner:" + receiver_.get_info().get_volume_name() + "\n");
        response += ("Free blocks:" + std::to_string(receiver_.get_catalog().get_free_space()) + "\n");
        response += ("Bad blocks:" + std::to_string(receiver_.get_catalog().get_blocked_space()) + "\n");
    }

    for (const auto& segment : receiver_.get_catalog().get_segments()) {
        for (const auto& record : segment.get_records()) {
            if (record.get_type() != filesystem::FileType::FREE && record.get_type() != filesystem::FileType::BLOCKED) {
                temp = std::format("{} {} Blocks {}", record.get_filename(), record.get_size(), record.get_timestamp());
                response += (temp + "\n");
            }
        }
    }
    return response;
}