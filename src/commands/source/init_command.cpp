#include "commands/init_command.hpp"

std::string InitCommand::execute(const nlohmann::json& json) {
    if (receiver_.is_valid()) {
        return ERROR + ": the file system has already been initialized";
    }
    auto data = json["data"].get<std::vector<std::string>>();
    if (data.empty()) {  // in validate
        return ERROR + ": the volume ID was not transmitted";
    }
    std::string volume = data[0], name = "";
    if (data.size() == 2) {
        name = data[1];
    }
    size_t segm = json["options"]["segm"].get<size_t>();
    size_t vol = json["options"]["vol"].get<size_t>();
    size_t rec = json["options"]["rec"].get<size_t>();
    receiver_.set_info({volume, name});
    receiver_.init_catalog(segm, rec, vol);
    receiver_.validate();
    return OK;
}