#pragma once

#include "abstract_command.hpp"

class SortCommand : public AbstractCommand {
   public:
    explicit SortCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute(const nlohmann::json& json) override;
};
