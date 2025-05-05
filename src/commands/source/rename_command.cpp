#include "commands/rename_command.hpp"

std::string RenameCommand::execute() {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }

    if (old_filename_ == new_filename_) {
        return OK + std::format(": the {} has been renamed", old_filename_);
    }
    if (receiver_.get_catalog().get_files().contains(new_filename_)) {
        return FILE_ALREADY_EXISTS;
    }
    auto record = receiver_.get_catalog().find_record(old_filename_);
    if (record) {
        record->set_filename(new_filename_);
        receiver_.get_catalog().get_files().erase(old_filename_);
        receiver_.get_catalog().get_files().insert(new_filename_);
        return OK + std::format(": the {} has been renamed", old_filename_);
    }
    return FILE_NOT_FOUND;
}
