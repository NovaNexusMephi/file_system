#include "commands/move_command.hpp"
#include "commands/commands_constants.hpp"
#include "commands/create_command.hpp"
#include "commands/delete_command.hpp"

std::string MoveCommand::execute() {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }
    auto& catalog = receiver_.get_catalog();
    if (catalog.get_files().contains(dist_filename_)) {
        return FILE_ALREADY_EXISTS;
    }
    auto record = catalog.find_record(filename_);
    if (record) {
        CreateCommand create(receiver_, dist_filename_, record->get_size());
        DeleteCommand delete_command(receiver_, filename_);
        create.execute();
        delete_command.execute();
        return OK + std::format(": the {} has been moved", dist_filename_);
    }
    return FILE_NOT_FOUND;
}
