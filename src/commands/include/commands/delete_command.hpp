#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class DeleteCommand : public AbstractCommand {
   public:
    explicit DeleteCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute(const nlohmann::json& json) override;
};