#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command representing an unrecognized command name.
 * 
 * Used when the input command name doesn't match any valid commands.
 * Part of the Command pattern implementation for consistent error handling.
 */
class UnknownCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs an UnknownCommand for unrecognized command names.
     * @param file Reference to the associated FileSystem instance.
     */
    explicit UnknownCommand(filesystem::FileSystem& file) : AbstractCommand(file) {}

    /**
     * @brief Returns a standardized error message for unknown commands.
     * 
     * @return Error message string explaining the unknown command.
     */
    std::string execute() override;
};