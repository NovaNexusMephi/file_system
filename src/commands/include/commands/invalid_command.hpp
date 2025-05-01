#pragma once

#include "abstract_command.hpp"

class InvalidCommand : public AbstractCommand {
   public:
    explicit InvalidCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute(const nlohmann::json& json) override;
};
