#include "command_builder/command_builder.hpp"

#include "commands/abstract_command.hpp"
#include "commands/add_command.hpp"
#include "commands/copy_command.hpp"
#include "commands/create_command.hpp"
#include "commands/delete_command.hpp"
#include "commands/dir_command.hpp"
#include "commands/exit_command.hpp"
#include "commands/free_command.hpp"
#include "commands/help_command.hpp"
#include "commands/init_command.hpp"
#include "commands/invalid_command.hpp"
#include "commands/move_command.hpp"
#include "commands/rename_command.hpp"
#include "commands/sort_command.hpp"
#include "commands/squeeze_command.hpp"
#include "commands/unknown_command.hpp"
#include "commands/vol_command.hpp"

std::unique_ptr<AbstractCommand> CommandBuilder::build(const nlohmann::json& json) {
    auto name = json["name"].get<std::string>();

    auto validation_result = validator_.validate(name);

    if (validation_result == ValidationResult::INVALID) {
        return std::make_unique<InvalidCommand>(receiver_);
    } else if (validation_result == ValidationResult::UNKNOWN_COMMAND) {
        return std::make_unique<UnknownCommand>(receiver_);
    }

    if (name == "dir") {
        return std::make_unique<DirCommand>(receiver_);
    } else if (name == "init") {
        return std::make_unique<InitCommand>(receiver_);
    } else if (name == "create") {
        return std::make_unique<CreateCommand>(receiver_);
    } else if (name == "delete") {
        return std::make_unique<DeleteCommand>(receiver_);
    } else if (name == "rename") {
        return std::make_unique<RenameCommand>(receiver_);
    } else if (name == "copy") {
        return std::make_unique<CopyCommand>(receiver_);
    } else if (name == "move") {
        return std::make_unique<MoveCommand>(receiver_);
    } else if (name == "add") {
        return std::make_unique<AddCommand>(receiver_);
    } else if (name == "squeeze") {
        return std::make_unique<SqueezeCommand>(receiver_);
    } else if (name == "sort") {
        return std::make_unique<SortCommand>(receiver_);
    } else if (name == "free") {
        return std::make_unique<FreeCommand>(receiver_);
    } else if (name == "vol") {
        return std::make_unique<VolCommand>(receiver_);
    } else if (name == "exit") {
        return std::make_unique<ExitCommand>(receiver_);
    } else if (name == "help") {
        return std::make_unique<HelpCommand>(receiver_);
    }

    return nullptr;
}
