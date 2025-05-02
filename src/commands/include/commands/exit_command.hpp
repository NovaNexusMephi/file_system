#pragma once

#include <exception>
#include <string>

#include "abstract_command.hpp"

class ExitException : public std::exception {
   public:
    explicit ExitException(const std::string& message) : msg_(message) {}

    virtual ~ExitException() noexcept override = default;

    const char* what() const noexcept override { return msg_.c_str(); }

   private:
    std::string msg_;
};

class ExitCommand : public AbstractCommand {
   public:
    explicit ExitCommand(filesystem::FileSystem& file) : AbstractCommand(file){};

    std::string execute() override;
};
