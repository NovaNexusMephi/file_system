#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command to copy an existing file to a new location.
 * 
 * Creates a duplicate of the specified file with a new name. Requires the 
 * source file to exist and the destination name to be available. Uses
 * CreateCommand internally to allocate space for the copy.
 */
class CopyCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs a CopyCommand for file duplication.
     * 
     * @param file Reference to the target FileSystem instance.
     * @param filename Source file name to copy.
     * @param dist Destination file name for the copy.
     */
    explicit CopyCommand(filesystem::FileSystem& file, const std::string& filename, const std::string& dist)
        : AbstractCommand(file), filename_(filename), dist_filename_(dist) {};

    /**
     * @brief Executes the file copy operation.
     * 
     * Execution steps:
     * 1. Validate file system initialization
     * 2. Check destination name availability
     * 3. Verify source file existence
     * 4. Create new file with source's size using CreateCommand
     * 
     * @return Status message indicating success or error reason.
     *         Possible returns: OK, ERROR, FILE_ALREADY_EXISTS, FILE_NOT_FOUND
     */
    std::string execute() override;

   private:
    std::string filename_;       ///< Source file name
    std::string dist_filename_;  ///< Destination file name
};