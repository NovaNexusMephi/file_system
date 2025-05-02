#pragma once

#include "abstract_command.hpp"

class HelpCommand : public AbstractCommand {
   public:
    explicit HelpCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute() override;
};
