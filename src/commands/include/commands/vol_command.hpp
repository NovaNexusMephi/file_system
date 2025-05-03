#pragma once

#include "abstract_command.hpp"

class VolCommand : public AbstractCommand {
   public:
    explicit VolCommand(filesystem::FileSystem& file, const std::string& vol, const std::string& ow = "") : 
        AbstractCommand(file), volume_(vol), owner_(ow) {};

    std::string execute() override;
private:
    std::string volume_;
    std::string owner_;
};