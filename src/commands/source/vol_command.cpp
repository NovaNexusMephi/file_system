#include "commands/vol_command.hpp"

#include "commands/commands_constants.hpp"

std::string VolCommand::execute(const nlohmann::json& json) {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }
    auto data = json["data"].get<std::vector<std::string>>();
    receiver_.get_info().set_volume_name(data[0]);
    if (data.size() == 2) {
        receiver_.get_info().set_owner_name(data[1]);
    }

    return OK + ": the volume ID has been changed";
}