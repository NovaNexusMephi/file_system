#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

class InitCommand : public AbstractCommand {
public:

    explicit InitCommand(filesystem::FileSystem& file) : AbstractCommand(file) {};

    std::string execute(const nlohmann::json& json) override;
};