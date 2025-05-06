#pragma once

#include "abstract_command.hpp"

/**
 * @brief Represents an invalid command that failed validation checks.
 * 
 * Used when input doesn't match any valid command structure. Part of the
 * command pattern implementation for error handling consistency.
 */
class InvalidCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs an InvalidCommand for syntax/structure errors.
     * @param file Reference to the associated FileSystem instance.
     */
    explicit InvalidCommand(filesystem::FileSystem& file) : AbstractCommand(file) {}

    /**
     * @brief Returns a standardized error message for invalid commands.
     * 
     * @return Error message string explaining the invalid command format.
     */
    std::string execute() override;
};