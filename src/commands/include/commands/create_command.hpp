#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class CreateCommand : public AbstractCommand {
public:

    explicit CreateCommand(filesystem::FileSystem& file) : AbstractCommand(file) {};

    std::string execute(const nlohmann::json& json) override;
};