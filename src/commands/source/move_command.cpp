#include "commands/move_command.hpp"

std::string MoveCommand::execute(const nlohmann::json& json) {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }
    auto data = json["data"].get<std::vector<std::string>>();
    std::string filename = data[0], dist_filename = data[1];
    auto& catalog = receiver_.get_catalog();
    if (catalog.get_files().contains(dist_filename)) {
        return FILE_ALREADY_EXISTS;
    }
    auto record = catalog.find_record(filename);
    if (record) {
        nlohmann::json create_json = {
            {"name", "create"}, {"data", {dist_filename}}, {"options", {{"allocate", record->get_size()}}}};
        nlohmann::json delete_json = {{"name", "delete"}, {"data", {filename}}};
        CreateCommand create(receiver_);
        DeleteCommand delete_command(receiver_);
        create.execute(create_json);
        delete_command.execute(delete_json);
        return OK + " the file has been moved";
    }
    return FILE_NOT_FOUND;
}
