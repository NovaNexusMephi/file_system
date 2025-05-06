#pragma once

#include <string>

/** Status and error message constants for command execution results */
static const std::string OK = "OK";                                    ///< Successful operation
static const std::string ERROR = "ERROR";                              ///< General operation failure
static const std::string NO_FREE_RECORDS = "NO_FREE_RECORDS";          ///< No available file records in catalog
static const std::string NO_FREE_SPACE = "NO_FREE_SPACE";              ///< Insufficient free space for operation
static const std::string FILE_NOT_FOUND = "FILE_NOT_FOUND";            ///< Requested file doesn't exist
static const std::string FILE_ALREADY_EXISTS = "FILE_ALREADY_EXISTS";  ///< File name conflict
static const std::string EXIT = "EXIT";                                ///< Termination signal

/** Command help documentation strings */
static const std::string HELP_DIR =
    "DIR\n"
    "Output catalog contents with optional filtering and result saving.\n"
    "Parameters:\n"
    "<full> [bool] - optional: display full file metadata\n"
    "Keys: -\n";

static const std::string HELP_INIT =
    "INIT\n"
    "Format disk with specified file system parameters.\n"
    "Parameters:\n"
    "<volume_id> [string] - required volume label\n"
    "<owner> [string] - optional owner name\n"
    "Keys:\n"
    "/segm:N [int] - segment count\n"
    "/rec:N [int] - records per segment\n"
    "/vol:N [int] - volume size (redundant with <volume_id>)\n";

static const std::string HELP_CREATE =
    "CREATE\n"
    "Create new file with preallocated space.\n"
    "Parameters:\n"
    "<filename> [string] - required file name\n"
    "Keys:\n"
    "/allocate:<size> [int] - required allocation size\n";

static const std::string HELP_DELETE =
    "DELETE\n"
    "Remove specified file.\n"
    "Parameters:\n"
    "<filename> [string] - required file name\n"
    "Keys: -\n";

static const std::string HELP_RENAME =
    "RENAME\n"
    "Change file name and/or extension.\n"
    "Parameters:\n"
    "<old_name> [string] - current file name\n"
    "<new_name> [string] - new file name\n"
    "Keys: -\n";

static const std::string HELP_COPY =
    "COPY\n"
    "Duplicate file within file system.\n"
    "Parameters:\n"
    "<source> [string] - source file name\n"
    "<destination> [string] - target file name\n"
    "Keys: -\n";

static const std::string HELP_MOVE =
    "MOVE\n"
    "Relocate file, replacing destination if exists.\n"
    "Parameters:\n"
    "<source> [string] - source file name\n"
    "<destination> [string] - target file name\n"
    "Keys: -\n";

static const std::string HELP_ADD =
    "ADD\n"
    "Extend existing file with additional blocks.\n"
    "Parameters:\n"
    "<filename> [string] - target file name\n"
    "Keys:\n"
    "/size:<blocks> [int] - number of blocks to add\n";

static const std::string HELP_SQUEEZE =
    "SQUEEZE\n"
    "Defragment free space (≤5 sec for 1MB volume).\n"
    "Parameters: -\n"
    "Keys: -\n";

static const std::string HELP_SORT =
    "SORT\n"
    "Display sorted catalog entries.\n"
    "Sort criteria (mutually exclusive):\n"
    "/name - by filename\n"
    "/ext - by extension\n"
    "/date - by creation date\n"
    "/size - by file size\n"
    "/inv - reverse order\n";

static const std::string HELP_FREE =
    "FREE\n"
    "Show free space information.\n"
    "Parameters: -\n"
    "Keys: -\n";

static const std::string HELP_VOL =
    "VOL\n"
    "Modify volume metadata.\n"
    "Parameters:\n"
    "<volume_id> [string] - new volume label\n"
    "<owner> [string] - new owner name\n"
    "Keys: -\n";

static const std::string HELP_EXIT =
    "EXIT\n"
    "Terminate file system session.\n"
    "Parameters: -\n"
    "Keys: -\n";

static const std::string HELP_HELP =
    "HELP\n"
    "Show command documentation.\n"
    "Parameters:\n"
    "<command> [string] - optional command name\n"
    "Keys: -\n";

/** Aggregated full help documentation */
static const std::string HELP_FULL = HELP_DIR + HELP_INIT + HELP_CREATE + HELP_DELETE + HELP_RENAME + HELP_COPY +
                                     HELP_MOVE + HELP_ADD + HELP_SQUEEZE + HELP_SORT + HELP_FREE + HELP_VOL +
                                     HELP_EXIT + HELP_HELP;