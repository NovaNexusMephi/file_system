#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class CopyCommand : public AbstractCommand {
   public:
    explicit CopyCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute(const nlohmann::json& json) override;
};
