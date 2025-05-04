#include "commands/help_command.hpp"

#include "commands/commands_constants.hpp"

std::string HelpCommand::execute() {
    if (command_ == "") {
        return OK + "\n" + HELP_FULL;
    } else if (command_ == "init") {
        return OK + "\n" + HELP_INIT;
    } else if (command_ == "create") {
        return OK + "\n" + HELP_CREATE;
    } else if (command_ == "delete") {
        return OK + "\n" + HELP_DELETE;
    } else if (command_ == "rename") {
        return OK + "\n" + HELP_RENAME;
    } else if (command_ == "copy") {
        return OK + "\n" + HELP_COPY;
    } else if (command_ == "move") {
        return OK + "\n" + HELP_MOVE;
    } else if (command_ == "add") {
        return OK + "\n" + HELP_ADD;
    } else if (command_ == "squeeze") {
        return OK + "\n" + HELP_SQUEEZE;
    } else if (command_ == "sort") {
        return OK + "\n" + HELP_SORT;
    } else if (command_ == "free") {
        return OK + "\n" + HELP_FREE;
    } else if (command_ == "dir") {
        return OK + "\n" + HELP_DIR;
    } else if (command_ == "vol") {
        return OK + "\n" + HELP_VOL;
    } else if (command_ == "exit") {
        return OK + "\n" + HELP_EXIT;
    } else if (command_ == "help") {
        return OK + "\n" + HELP_HELP;
    }
    return ERROR + ": unknown command";
}
