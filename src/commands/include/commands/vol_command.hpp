#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command to modify the file system's volume label and owner information.
 * 
 * Updates the SystemInformation metadata without affecting file contents.
 * Follows the Command design pattern for system configuration operations.
 */
class VolCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs a VolCommand with new volume/owner information.
     * 
     * @param file Reference to the target FileSystem instance.
     * @param vol New volume label/name.
     * @param ow Optional new owner name (default: keep existing).
     */
    explicit VolCommand(filesystem::FileSystem& file, const std::string& vol, const std::string& ow = "")
        : AbstractCommand(file), volume_(vol), owner_(ow) {}

    /**
     * @brief Executes the volume information update.
     * 
     * Process steps:
     * 1. Validate file system initialization
     * 2. Update volume name in system metadata
     * 3. Update owner name if provided
     * 
     * @return Status message indicating success or error reason.
     *         Possible returns: OK with confirmation, ERROR if uninitialized.
     */
    std::string execute() override;

   private:
    std::string volume_;  ///< New volume label
    std::string owner_;   ///< Optional new owner name
};