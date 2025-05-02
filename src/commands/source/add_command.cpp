#include "commands/add_command.hpp"

std::string AddCommand::execute() {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }
    auto& catalog = receiver_.get_catalog();
    auto record = catalog.find_record(filename_);
    if (record) {
        auto size = record->get_size();
        if (size_ > catalog.get_free_space()) {
            return NO_FREE_SPACE;
        }
        record->set_size(size + size_);
        catalog.get_free_space() -= size_;
        return OK + ": the file size has been increased";
    }
    return FILE_NOT_FOUND;
}
