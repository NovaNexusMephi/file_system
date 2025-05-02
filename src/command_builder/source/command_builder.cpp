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

    auto validation_result = validator_.validate(name, json);

    if (validation_result == ValidationResult::INVALID) {
        return std::make_unique<InvalidCommand>(receiver_);
    } else if (validation_result == ValidationResult::UNKNOWN_COMMAND) {
        return std::make_unique<UnknownCommand>(receiver_);
    }

    if (name == "dir") {
        auto data = json["data"].get<std::vector<std::string>>();
        bool full = !data.empty();
        return std::make_unique<DirCommand>(receiver_, full);
    } else if (name == "init") {
        auto data = json["data"].get<std::vector<std::string>>();
        std::string volume = data[0];
        std::string owner = data.size() > 1 ? data[1] : "";
        size_t segm = json["options"]["segm"].get<size_t>();
        size_t rec = json["options"]["rec"].get<size_t>();
        size_t vol = json["options"]["vol"].get<size_t>();
        return std::make_unique<InitCommand>(receiver_, volume, owner, segm, rec, vol);
    } else if (name == "create") {
        auto data = json["data"].get<std::vector<std::string>>();
        std::string filename = data[0];
        size_t size = json["options"]["allocate"].get<size_t>();
        return std::make_unique<CreateCommand>(receiver_, filename, size);
    } else if (name == "delete") {
        auto data = json["data"].get<std::vector<std::string>>();
        std::string filename = data[0];
        return std::make_unique<DeleteCommand>(receiver_, filename);
    } else if (name == "rename") {
        auto data = json["data"].get<std::vector<std::string>>();
        std::string old_name = data[0];
        std::string new_name = data[1];
        return std::make_unique<RenameCommand>(receiver_, old_name, new_name);
    } else if (name == "copy") {
        auto data = json["data"].get<std::vector<std::string>>();
        std::string source = data[0];
        std::string destination = data[1];
        return std::make_unique<CopyCommand>(receiver_, source, destination);
    } else if (name == "move") {
        auto data = json["data"].get<std::vector<std::string>>();
        std::string source = data[0];
        std::string destination = data[1];
        return std::make_unique<MoveCommand>(receiver_, source, destination);
    } else if (name == "add") {
        auto data = json["data"].get<std::vector<std::string>>();
        std::string filename = data[0];
        size_t size = json["options"]["size"].get<size_t>();
        return std::make_unique<AddCommand>(receiver_, filename, size);
    } else if (name == "squeeze") {
        return std::make_unique<SqueezeCommand>(receiver_);
    } else if (name == "sort") {
        auto data = json["data"].get<std::vector<std::string>>();
        std::string sort_by = data[0];
        std::string inv = data.size() > 1 ? data[1] : "";
        return std::make_unique<SortCommand>(receiver_, sort_by, inv);
    } else if (name == "free") {
        return std::make_unique<FreeCommand>(receiver_);
    } else if (name == "vol") {
        auto data = json["data"].get<std::vector<std::string>>();
        std::string volume = data[0];
        std::string owner = data.size() > 1 ? data[1] : "";
        return std::make_unique<VolCommand>(receiver_, volume, owner);
    } else if (name == "exit") {
        return std::make_unique<ExitCommand>(receiver_);
    } else if (name == "help") {
        auto data = json["data"].get<std::vector<std::string>>();
        std::string command = data.size() > 0 ? data[0] : "";
        return std::make_unique<HelpCommand>(receiver_, command);
    }

    return nullptr;
}
