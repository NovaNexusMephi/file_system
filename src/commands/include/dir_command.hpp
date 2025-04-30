#pragma once

#include "abstract_command.hpp"

#include <format>

class DirCommand : public AbstractCommand {

public:
    explicit DirCommand(const filesystem::FileSystem& file) : AbstractCommand(file) {};

    std::string execute(const nlohmann::json& json) override;

};