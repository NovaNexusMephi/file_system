#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class CreateCommand : public AbstractCommand {
public:

    explicit CreateCommand(filesystem::FileSystem& file, const std::string& str, size_t size) : 
            AbstractCommand(file), filename_(str), size_(size) {};

    std::string execute() override;
private:
    std::string filename_;
    size_t size_;
};