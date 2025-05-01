#pragma once

#include <memory>
#include <nlohmann/json.hpp>

#include "commands/abstract_command.hpp"
#include "validator.hpp"

class CommandBuilder {
   public:
    explicit CommandBuilder(filesystem::FileSystem& receiver) : receiver_(receiver) {}

    std::unique_ptr<AbstractCommand> build(const nlohmann::json& json);

   private:
    Validator validator_;
    filesystem::FileSystem& receiver_;
};
