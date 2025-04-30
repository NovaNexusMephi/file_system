#pragma once

#include <nlohmann/json.hpp>

#include "filesystem/filesystem.hpp"

//пока что будут здесь константы, я за то, чтобы вынести их в отдельный файл

static const std::string OK = "OK";
static const std::string ERROR = "ERROR";

class AbstractCommand {
   public:
    virtual std::string execute(const nlohmann::json& json) = 0;

    virtual ~AbstractCommand() = default;

   protected:

    explicit AbstractCommand(filesystem::Filesystem& receiver) : receiver_(receiver) {}

    filesystem::Filesystem& receiver_;
};