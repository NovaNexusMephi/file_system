#pragma once

#include "abstract_command.hpp"
#include "commands_constants.hpp"

#include <format>

class DirCommand : public AbstractCommand {

public:
    explicit DirCommand(filesystem::FileSystem& file) : AbstractCommand(file) {};

    std::string execute(const nlohmann::json& json) override;

};