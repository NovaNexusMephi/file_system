#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command to create a new file with specified allocation size.
 * 
 * Handles file creation by finding contiguous or non-contiguous free blocks
 * in the file system. Updates catalog metadata and manages block allocation
 * states (PERMANENT/BLOCKED). Follows the Command design pattern.
 */
class CreateCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs a CreateCommand for file creation.
     * 
     * @param file Reference to the target FileSystem instance.
     * @param str Filename to create.
     * @param size Allocation size in bytes.
     */
    explicit CreateCommand(filesystem::FileSystem& file, const std::string& str, size_t size)
        : AbstractCommand(file), filename_(str), size_(size) {};

    /**
     * @brief Executes the file creation process.
     * 
     * The algorithm performs these operations:
     * 1. Validate file system state
     * 2. Check for filename conflicts
     * 3. Verify available space and records
     * 4. Attempt contiguous block allocation in existing segments
     * 5. If failed, try non-contiguous allocation in new segments
     * 6. Update catalog metadata and block states
     * 
     * @return Status message indicating success or error reason.
     *         Possible returns: OK, ERROR, FILE_ALREADY_EXISTS,
     *         NO_FREE_SPACE, NO_FREE_RECORDS
     */
    std::string execute() override;

   private:
    std::string filename_;  ///< Name of the new file
    size_t size_;           ///< Allocation size in bytes
};