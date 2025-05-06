#pragma once

#include <string>
#include "abstract_command.hpp"

/**
 * @brief Command to move/rename a file, replacing any existing destination.
 * 
 * Implements move operation through copy-delete sequence. Requires source
 * file existence and destination name availability. Follows the Command
 * design pattern for operation consistency.
 */
class MoveCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs a MoveCommand for file relocation.
     * 
     * @param file Reference to the target FileSystem instance.
     * @param filename Source file name to move.
     * @param dist Destination file name/path.
     */
    explicit MoveCommand(filesystem::FileSystem& file, const std::string& filename, const std::string& dist)
        : AbstractCommand(file), filename_(filename), dist_filename_(dist) {};

    /**
     * @brief Executes the file move operation.
     * 
     * The process follows these steps:
     * 1. Validate file system initialization
     * 2. Check destination name availability
     * 3. Verify source file existence
     * 4. Create copy at destination using CreateCommand
     * 5. Delete original file using DeleteCommand
     * 
     * @return Status message indicating success or error reason.
     *         Possible returns: OK, ERROR, FILE_ALREADY_EXISTS,
     *         FILE_NOT_FOUND
     */
    std::string execute() override;

   private:
    std::string filename_;       ///< Source file name
    std::string dist_filename_;  ///< Destination file name
};