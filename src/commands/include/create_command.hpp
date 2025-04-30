#pragma once

#include "abstract_command.hpp"

class CreateCommand : public AbstractCommand {
public:

    explicit CreateCommand(const filesystem::FileSystem& file) : AbstractCommand(file) {};

    /*inline void execute() override {
        receiver_.
    }*/

};