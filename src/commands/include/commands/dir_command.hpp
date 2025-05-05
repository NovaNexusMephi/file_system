#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

#include <format>

class DirCommand : public AbstractCommand {

public:
    explicit DirCommand(filesystem::FileSystem& file, bool full = false) : 
        AbstractCommand(file), full_(full) {};

    std::string execute() override;

private:
    bool full_;

};