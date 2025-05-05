#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class SqueezeCommand : public AbstractCommand {
   public:
    explicit SqueezeCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute() override;
};
