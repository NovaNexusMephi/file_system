#include "commands/init_command.hpp"

std::string InitCommand::execute() {
    if (receiver_.is_valid()) {
        return ERROR + ": the file system has already been initialized";
    }
    receiver_.set_info({volume_, owner_});
    receiver_.init_catalog(segm_, rec_, vol_);
    receiver_.validate();
    return OK;
}