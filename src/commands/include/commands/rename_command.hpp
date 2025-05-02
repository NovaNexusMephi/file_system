#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class RenameCommand : public AbstractCommand {
   public:
    explicit RenameCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute(const nlohmann::json& json) override;
};
