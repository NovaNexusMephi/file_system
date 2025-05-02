#pragma once

#include <string>

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class InitCommand : public AbstractCommand {
   public:
    explicit InitCommand(filesystem::FileSystem& file, const std::string& v, const std::string& ow, size_t segm,
                         size_t rec, size_t vol)
        : AbstractCommand(file), volume_(v), owner_(ow), segm_(segm), vol_(vol), rec_(rec){};

    std::string execute() override;

   private:
    std::string volume_;
    std::string owner_;
    size_t segm_;
    size_t vol_;
    size_t rec_;
};