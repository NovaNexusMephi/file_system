#pragma once

#include "abstract_command.hpp"

class ExitCommand : public AbstractCommand {
   public:
    explicit ExitCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute() override;
};
