#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command to increase the allocated size of an existing file.
 * 
 * This command adds additional storage space to a specified file if sufficient
 * free space exists in the file system. Follows the Command design pattern.
 */
class AddCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs an AddCommand for modifying file allocation.
     * 
     * @param file Reference to the target FileSystem instance.
     * @param filename Name of the file to modify.
     * @param size Number of bytes to add to the file's allocation.
     */
    explicit AddCommand(filesystem::FileSystem& file, const std::string& filename, size_t size)
        : AbstractCommand(file), filename_(filename), size_(size) {};

    /**
     * @brief Executes the file size modification operation.
     * 
     * The execution follows these steps:
     * 1. Check file system initialization status
     * 2. Verify file existence in the catalog
     * 3. Check available free space
     * 4. Update file allocation and system metadata
     * 
     * @return Status message indicating success or error reason.
     */
    std::string execute() override;

   private:
    std::string filename_;  ///< Name of the target file
    size_t size_;           ///< Size increment in bytes
};