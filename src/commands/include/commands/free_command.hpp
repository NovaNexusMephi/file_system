#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command to display detailed free space information.
 * 
 * Shows locations and sizes of free blocks in the file system, including both
 * allocated segment space and direct free space. Follows the Command pattern.
 */
class FreeCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs a FreeCommand for the specified file system.
     * @param file Reference to the target FileSystem instance.
     */
    explicit FreeCommand(filesystem::FileSystem& file) : AbstractCommand(file) {}

    /**
     * @brief Executes the free space reporting operation.
     * 
     * The output format includes:
     * 1. Numbered list of free block ranges
     * 2. Total occupied space
     * 3. Total free space
     * 
     * Example output:
     * OK:
     * 1. 100 512
     * 2. 613 256
     * Occupied: 8192
     * Free: 1024
     * 
     * @return Formatted free space report or error message.
     *         Possible returns: OK with details, ERROR if uninitialized.
     */
    std::string execute() override;
};