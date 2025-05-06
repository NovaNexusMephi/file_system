#pragma once

#include <exception>
#include <string>

#include "abstract_command.hpp"

/**
 * @brief Exception class specifically for signaling application exit.
 * 
 * Carries an exit status message to be handled by the main application loop.
 */
class ExitException : public std::exception {
   public:
    /**
     * @brief Constructs an exit exception with a status message.
     * @param message Exit status message (typically OK or ERROR).
     */
    explicit ExitException(const std::string& message) : msg_(message) {}

    /**
     * @brief Returns the exit status message.
     * @return C-string representation of the status message.
     */
    const char* what() const noexcept override { return msg_.c_str(); }

   private:
    std::string msg_;  ///< Exit status message
};

/**
 * @brief Command to terminate the file system interface session.
 * 
 * This command uses exception-based control flow to immediately exit the
 * application. Follows the Command design pattern but overrides the
 * standard execution behavior.
 */
class ExitCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs an ExitCommand for the specified file system.
     * @param file Reference to the associated FileSystem instance.
     */
    explicit ExitCommand(filesystem::FileSystem& file) : AbstractCommand(file) {}

    /**
     * @brief Triggers application exit by throwing an ExitException.
     * 
     * This method does NOT return normally. Instead, it always throws
     * an exception to propagate the exit request to the application's
     * top-level error handler.
     * 
     * @return Never returns (always throws)
     * @throws ExitException with OK status message
     */
    std::string execute() override;
};