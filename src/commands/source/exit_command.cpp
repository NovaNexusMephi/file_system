#include "commands/exit_command.hpp"

std::string ExitCommand::execute(const nlohmann::json& json) { return "exit"; }
