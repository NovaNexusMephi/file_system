#include "init_command.hpp"

std::string InitCommand::execute(const nlohmann::json& json) {
    
    if(receiver_.get_valid()) {
        return ERROR + ": the file system has already been initialized";
    }

    auto data = json["data"].get<std::vector<std::string>>();
    if(data.size() == 0) { //если я верно понял Ваню
        return ERROR + ": the volume ID was not transmitted";
    }
    std::string volume = data[0], name = "";
    if(data.size() == 2) {   //maybe in validate
        name = data[1];
    }
    size_t segm = json["options"]["segm"].get<size_t>();
    size_t vol = json["options"]["vol"].get<size_t>();
    size_t rec = json["options"]["rec"].get<size_t>();
    filesystem::SystemInformation sys(volume, name);
    filesystem::Catalog catalog(segm, rec, vol);
    receiver_.set_info(sys);
    receiver_.set_catalog(catalog);
    receiver_.set_valid(true);
    return OK;
}