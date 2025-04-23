#pragma once

#include <nlohmann/json.hpp>

#include "filesystem/filesystem.hpp"

class AbstractCommand {
   public:
    virtual void execute() = 0;
    virtual ~AbstractCommand() = default;
    bool validate();

   protected:
    nlohmann::json validation_schema_;
    filesystem::Filesystem& receiver_;
};