#pragma once

#include <string>

static const std::string OK = "OK";
static const std::string ERROR = "ERROR";
static const std::string NO_FREE_RECORDS = "NO_FREE_RECORDS";
static const std::string NO_FREE_SPACE = "NO_FREE_SPACE";
static const std::string FILE_NOT_FOUND = "FILE_NOT_FOUND";
static const std::string FILE_ALREADY_EXISTS = "FILE_ALREADY_EXISTS";
static const std::string EXIT = "EXIT";

static const std::string HELP_DIR =
    "DIR\nOutput of the catalog contents with the ability to filter and save the result.\nParameters:\n<full> [bool] - "
    "optional: display full files info.\nKeys: -\n";
static const std::string HELP_INIT =
    "INIT\nFormat a disk to create a file system with specified parameters.\nParameters:\n<volume_id> [string] - "
    "mandatory: volume label.\n<owner> [string] - optional: owner name.\nKeys:\n/segm:N [int] - optional: number of "
    "catalog segments (default: calculated).\n/rec:N [int] - optional: number of entries per segment.\n/vol:N [int] - "
    "mandatory: volume label (redundant with <volume_id>).\n";
static const std::string HELP_CREATE =
    "CREATE\nCreate a new file with preallocated space.\nParameters:\n<filename> [string] - mandatory: name of the "
    "file.\nKeys:\n/allocate:<size> [int] - mandatory: number of blocks to allocate.\n";
static const std::string HELP_DELETE =
    "DELETE\nDelete a file.\nParameters:\n<filename> [string] - mandatory: name of the file to delete.\nKeys: -\n";
static const std::string HELP_RENAME =
    "RENAME\nRename a file, including its name and extension.\nParameters:\n<old_name> [string] - mandatory: current "
    "file name.\n<new_name> [string] - mandatory: new file name.\nKeys: -\n";
static const std::string HELP_COPY =
    "COPY\nCopy a file within the file system.\nParameters:\n<source> [string] - mandatory: source file "
    "name.\n<destination> [string] - mandatory: target file name.\nKeys: -\n";
static const std::string HELP_MOVE =
    "MOVE\nMove a file, replacing the target file if it exists.\nParameters:\n<source> [string] - mandatory: source "
    "file name.\n<destination> [string] - mandatory: target file name.\nKeys: -\n";
static const std::string HELP_ADD =
    "ADD\nAppend data to the end of an existing file.\nParameters:\n<filename> [string] - mandatory: name of the "
    "file.\nKeys:\n/size:<blocks> [int] - mandatory: number of blocks to add.\n";
static const std::string HELP_SQUEEZE =
    "SQUEEZE\nDefragment the file system by merging free blocks. Execution time ≤5 sec for 1 MB.\nParameters: "
    "-\nKeys: -\n";
static const std::string HELP_SORT =
    "SORT\nDisplay the catalog sorted by name, extension, date, or size. Only one criterion allowed.\nParameters: "
    "-\nKeys:\n/name [bool] - optional: sort by filename.\n/ext [bool] - optional: sort by extension.\n/date [bool] - "
    "optional: sort by creation date.\n/size [bool] - optional: sort by file size.\n/inv [bool] - optional: reverse "
    "sorting order.\n";
static const std::string HELP_FREE =
    "FREE\nDisplay information about free disk space, including block locations and sizes.\nParameters: -\nKeys: -\n";
static const std::string HELP_VOL =
    "VOL\nModify the volume label and owner name.\nParameters:\n<volume_id> [string] - mandatory: new volume "
    "label.\n<owner> [string] - optional: new owner name.\nKeys: -\n";
static const std::string HELP_EXIT = "EXIT\nExit the file system interface.\nParameters: -\nKeys: -\n";
static const std::string HELP_HELP =
    "HELP\nDisplay a list of commands or detailed help for a specific command.\nParameters:\n<command> [string] - "
    "optional: command name for detailed help.\nKeys: -\n";

static const std::string HELP_FULL = HELP_DIR + HELP_INIT + HELP_CREATE + HELP_DELETE + HELP_RENAME + HELP_COPY +
                                     HELP_MOVE + HELP_ADD + HELP_SQUEEZE + HELP_SORT + HELP_FREE + HELP_VOL +
                                     HELP_EXIT + HELP_HELP;
