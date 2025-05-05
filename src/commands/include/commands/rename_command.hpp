#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class RenameCommand : public AbstractCommand {
   public:
    explicit RenameCommand(filesystem::FileSystem& file, const std::string& old, const std::string& new_): 
        AbstractCommand(file), old_filename_(old), new_filename_(new_) {};

    std::string execute() override;
private:
    std::string old_filename_;
    std::string new_filename_;
};
