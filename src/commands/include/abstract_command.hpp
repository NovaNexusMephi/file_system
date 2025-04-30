#pragma once

#include <nlohmann/json.hpp>

#include "filesystem/filesystem.hpp"

class AbstractCommand {
   public:
    virtual void execute() = 0;

    virtual ~AbstractCommand() = default;

   protected:

    explicit AbstractCommand(filesystem::Filesystem& receiver) : receiver_(receiver) {}

    filesystem::Filesystem& receiver_;
};