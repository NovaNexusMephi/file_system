#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command to retrieve help information about available commands.
 * 
 * Supports both general help (all commands) and specific command documentation.
 * Follows the Command design pattern for consistent interface implementation.
 */
class HelpCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs a HelpCommand with optional command-specific documentation.
     * 
     * @param file Reference to the associated FileSystem instance.
     * @param command Name of the command to document (empty for general help).
     */
    explicit HelpCommand(filesystem::FileSystem& file, const std::string& command)
        : AbstractCommand(file), command_(command) {}

    /**
     * @brief Executes the help documentation retrieval.
     * 
     * Behavior:
     * - Empty command: Returns full documentation (HELP_FULL)
     * - Valid command: Returns specific command documentation
     * - Invalid command: Returns error message
     * 
     * @return Formatted help documentation or error message.
     *         Possible returns: OK with help text, ERROR for unknown commands.
     */
    std::string execute() override;

   private:
    std::string command_;  ///< Target command for documentation (empty = general help)
};