#pragma once

#include <nlohmann/json.hpp>

#include "filesystem/filesystem.hpp"

/**
 * @brief Abstract base class for all file system commands following the Command pattern.
 * 
 * Defines the interface for command execution and holds a reference to the target file system.
 * All concrete command classes must inherit from this and implement the execute() method.
 */
class AbstractCommand {
   public:
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived command objects.
     */
    virtual ~AbstractCommand() = default;

    /**
     * @brief Pure virtual method for executing the command.
     * 
     * @return std::string Result/status message from command execution.
     */
    virtual std::string execute() = 0;

   protected:
    /**
     * @brief Constructs a command with the target file system receiver.
     * 
     * @param receiver Reference to the FileSystem instance to operate on.
     */
    explicit AbstractCommand(filesystem::FileSystem& receiver) : receiver_(receiver) {}

    filesystem::FileSystem& receiver_;  ///< Reference to the target file system
};