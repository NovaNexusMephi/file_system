#include "commands/copy_command.hpp"
#include "commands/create_command.hpp"

std::string CopyCommand::execute() {
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
        return create.execute();
    }
    return FILE_NOT_FOUND;
}
