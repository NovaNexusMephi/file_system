#pragma once

#include "abstract_command.hpp"

class MoveCommand : public AbstractCommand {
   public:
    explicit MoveCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute(const nlohmann::json& json) override;
};
