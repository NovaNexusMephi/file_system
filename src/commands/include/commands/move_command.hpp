#pragma once

#include <string>
#include "abstract_command.hpp"
#include "commands_constants.hpp"
#include "create_command.hpp"
#include "delete_command.hpp"

class MoveCommand : public AbstractCommand {
   public:
    explicit MoveCommand(filesystem::FileSystem& file, const std::string& filename, const std::string& dist) : 
            AbstractCommand(file), filename_(filename), dist_filename_(dist) {};

    std::string execute() override;
private:
    std::string filename_;
    std::string dist_filename_;
};
