#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class CopyCommand : public AbstractCommand {
   public:
    explicit CopyCommand(filesystem::FileSystem& file, const std::string& filename, const std::string& dist) : 
            AbstractCommand(file), filename_(filename), dist_filename_(dist) {};

    std::string execute() override;
private:
    std::string filename_;
    std::string dist_filename_;
};
