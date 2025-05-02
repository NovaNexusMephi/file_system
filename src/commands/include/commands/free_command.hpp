#pragma once

#include "abstract_command.hpp"

class FreeCommand : public AbstractCommand {
   public:
    explicit FreeCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute() override;
};
