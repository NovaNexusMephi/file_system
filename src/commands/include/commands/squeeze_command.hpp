#pragma once

#include "abstract_command.hpp"

class SqueezeCommand : public AbstractCommand {
   public:
    explicit SqueezeCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute(const nlohmann::json& json) override;
};
