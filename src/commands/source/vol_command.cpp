#include "commands/vol_command.hpp"

#include "commands/commands_constants.hpp"

std::string VolCommand::execute() {
    if (!receiver_.is_valid()) {
        return ERROR + ": the file system has not been initialized";
    }
    receiver_.get_info().set_volume_name(volume_);
    if(owner_ != "")
        receiver_.get_info().set_owner_name(owner_);

    return OK + ": the volume ID has been changed";
}