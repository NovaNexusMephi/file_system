#pragma once

#include "abstract_command.hpp"

class VolCommand : public AbstractCommand {
    
    explicit VolCommand(filesystem::FileSystem& file) : AbstractCommand(file) {};

    std::string execute(const nlohmann::json& json) override;
};