#pragma once

#include <nlohmann/json.hpp>

#include "filesystem/filesystem.hpp"

class AbstractCommand {
   public:
    virtual std::string execute() = 0;

    virtual ~AbstractCommand() = default;

   protected:
    explicit AbstractCommand(filesystem::FileSystem& receiver) : receiver_(receiver) {}

    filesystem::FileSystem& receiver_;
};