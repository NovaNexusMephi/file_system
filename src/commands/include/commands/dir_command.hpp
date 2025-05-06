#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command to list directory contents with optional detailed information.
 * 
 * Outputs file system metadata and file records based on the 'full' parameter.
 * Follows the Command design pattern for file system operations.
 */
class DirCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs a DirCommand with output detail control.
     * 
     * @param file Reference to the target FileSystem instance.
     * @param full If true, include volume metadata and system statistics.
     */
    explicit DirCommand(filesystem::FileSystem& file, bool full = false) : AbstractCommand(file), full_(full) {};

    /**
     * @brief Executes the directory listing operation.
     * 
     * Output format when full=true:
     * Volume: <name>, Owner: <name>
     * Free blocks: <number>
     * Bad blocks: <number>
     * <filename> <size> <timestamp>
     * ...
     * 
     * Output format when full=false:
     * <filename> <size> <timestamp>
     * ...
     * 
     * @return Formatted directory listing or error message.
     *         Possible returns: OK with listing, ERROR if uninitialized.
     */
    std::string execute() override;

   private:
    bool full_;  ///< Controls inclusion of system metadata in output
};