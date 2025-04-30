#pragma once

#include "abstract_command.hpp"

class InitCommand : public AbstractCommand {
public:

    explicit InitCommand(const filesystem::FileSystem& file) : AbstractCommand(file) {};

    std::string execute(const nlohmann::json& json) override;
};