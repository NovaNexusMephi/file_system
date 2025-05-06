#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command to delete an existing file and release its allocated space.
 * 
 * Handles both file record deletion and storage space management. Updates
 * catalog metadata and may adjust segment allocation states. Follows the
 * Command design pattern.
 */
class DeleteCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs a DeleteCommand for file removal.
     * 
     * @param file Reference to the target FileSystem instance.
     * @param filename Name of the file to delete.
     */
    explicit DeleteCommand(filesystem::FileSystem& file, const std::string& filename)
        : AbstractCommand(file), filename_(filename) {};

    /**
     * @brief Executes the file deletion process.
     * 
     * The operation performs these steps:
     * 1. Validate file system initialization
     * 2. Locate the file record in the catalog
     * 3. Mark record as FREE and update metadata
     * 4. Release allocated space back to free pool
     * 5. Optimize segment usage by consolidating free space
     * 
     * @return Status message indicating success or error reason.
     *         Possible returns: OK, ERROR, FILE_NOT_FOUND
     */
    std::string execute() override;

   private:
    std::string filename_;  ///< Name of the file to delete
};