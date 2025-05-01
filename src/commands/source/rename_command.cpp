#include "commands/rename_command.hpp"

std::string RenameCommand::execute(const nlohmann::json& json) {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }

    auto data = json["data"].get<std::vector<std::string>>();
    std::string old_filename = data[0];
    std::string new_filename = data[1];

    if (old_filename == new_filename) {
        return OK + ": the file has been renamed";
    }
    if (receiver_.get_catalog().get_files().contains(new_filename)) {
        return FILE_ALREADY_EXISTS;
    }
    auto record = receiver_.get_catalog().find_record(old_filename);
    if (record) {
        record->set_filename(new_filename);
        receiver_.get_catalog().get_files().erase(old_filename);
        receiver_.get_catalog().get_files().insert(new_filename);
        return OK + ": the file has been renamed";
    }
    return FILE_NOT_FOUND;
}
