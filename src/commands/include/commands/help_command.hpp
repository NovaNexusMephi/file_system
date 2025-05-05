#pragma once

#include "abstract_command.hpp"

class HelpCommand : public AbstractCommand {
   public:
    explicit HelpCommand(filesystem::FileSystem& file, const std::string& command)
        : AbstractCommand(file), command_(command){};

    std::string execute() override;

   private:
    std::string command_;
};
