#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class DeleteCommand : public AbstractCommand {
   public:
    explicit DeleteCommand(filesystem::FileSystem& file, const std::string& filename) : 
        AbstractCommand(file), filename_(filename) {};

    std::string execute() override;
private:
    std::string filename_;
};