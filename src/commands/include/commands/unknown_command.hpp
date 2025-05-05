#pragma once

#include "abstract_command.hpp"

class UnknownCommand : public AbstractCommand {
   public:
    explicit UnknownCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute() override;
};
