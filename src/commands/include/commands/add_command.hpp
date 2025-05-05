#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class AddCommand : public AbstractCommand {
   public:
    explicit AddCommand(filesystem::FileSystem& file, const std::string& filename, size_t size) : 
        AbstractCommand(file), filename_(filename), size_(size) {};

    std::string execute() override;
private:
    std::string filename_;
    size_t size_;
};
