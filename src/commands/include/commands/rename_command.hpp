#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command to rename an existing file in the file system.
 * 
 * Updates both the file record and catalog's filename registry. Follows the
 * Command design pattern for consistent operation handling.
 */
class RenameCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs a RenameCommand for file renaming.
     * 
     * @param file Reference to the target FileSystem instance.
     * @param old Original filename to be changed.
     * @param new_ New filename to assign.
     */
    explicit RenameCommand(filesystem::FileSystem& file, const std::string& old, const std::string& new_)
        : AbstractCommand(file), old_filename_(old), new_filename_(new_) {};

    /**
     * @brief Executes the rename operation.
     * 
     * The process follows these steps:
     * 1. Validate file system initialization
     * 2. Check for no-op (same name)
     * 3. Verify new name availability
     * 4. Update file record and catalog registry
     * 
     * @return Status message indicating success or error reason.
     *         Possible returns: OK, ERROR, FILE_ALREADY_EXISTS,
     *         FILE_NOT_FOUND
     */
    std::string execute() override;

   private:
    std::string old_filename_;  ///< Current filename
    std::string new_filename_;  ///< Requested new filename
};