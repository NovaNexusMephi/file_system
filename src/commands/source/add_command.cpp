#include "commands/add_command.hpp"

std::string AddCommand::execute(const nlohmann::json& json) {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }
    auto data = json["data"].get<std::vector<std::string>>();
    std::string filename = data[0];
    size_t new_size = json["options"]["size"].get<size_t>();
    auto& catalog = receiver_.get_catalog();
    auto record = catalog.find_record(filename);
    if(record) {
        auto size = record->get_size();
        if(new_size > catalog.get_free_space()) {
            return NO_FREE_SPACE;
        }
        record->set_size(size + new_size);
        catalog.get_free_space() -= new_size;
        return OK + ": the file size has been increased";
    }
    return FILE_NOT_FOUND;
}
