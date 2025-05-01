#include "commands/exit_command.hpp"
#include "commands/commands_constants.hpp"

std::string ExitCommand::execute(const nlohmann::json& json) {
    return EXIT;
}
