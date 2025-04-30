#include "dir_command.hpp"
#include "filesystem/file_record.hpp"

std::string DirCommand::execute(const nlohmann::json& json) {

    auto flag = json["options"]["full"].get<bool>();
    std::vector<std::string> result;
    std::string temp;
    if (flag) {
        result.emplace_back("Volume:" + receiver_.get_info().get_volume_name() + ", Owner:" + receiver_.get_info().get_volume_name());
        result.emplace_back("Free blocks:" + std::to_string(receiver_.get_catalog().get_free_space()));
        result.emplace_back("Bad blocks:" + std::to_string(receiver_.get_catalog().get_blocked_space()));
    }
    
    for (const auto& segment : receiver_.get_catalog().get_segments()) {
        for (const auto& record : segment.get_records()) {
            if (record.get_type() != filesystem::FileType::FREE && record.get_type() != filesystem::FileType::BLOCKED) {
                temp = std::format("{} {} Blocks {}", record.get_filename(), record.get_size(), record.get_timestamp());
                result.emplace_back(std::move(temp));
            }
        }
    }
    //ну типо понятно, что reponse будет OK, но с этим вектором че делать?
    //return result;
}