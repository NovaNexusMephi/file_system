#pragma once

#include "abstract_command.hpp"

class AddCommand : public AbstractCommand {
   public:
    explicit AddCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute(const nlohmann::json& json) override;
};
