#include "commands/exit_command.hpp"

#include <stdexcept>

#include "commands/commands_constants.hpp"

std::string ExitCommand::execute() {
    throw ExitException(OK);
}
