#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command to defragment the file system by consolidating free space.
 * 
 * Reorganizes storage segments to eliminate fragmentation and maximize
 * contiguous free space. Follows the Command design pattern for maintenance
 * operations.
 */
class SqueezeCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs a SqueezeCommand for the specified file system.
     * @param file Reference to the target FileSystem instance.
     */
    explicit SqueezeCommand(filesystem::FileSystem& file) : AbstractCommand(file) {}

    /**
     * @brief Executes the defragmentation process.
     * 
     * The operation follows these steps:
     * 1. Validate file system initialization
     * 2. Check if defragmentation is needed (no free space = no-op)
     * 3. Create new segment structure with compacted records
     * 4. Update catalog metadata:
     *    - Reset blocked space
     *    - Update free space tracking
     *    - Adjust segment usage counters
     * 5. Replace old segments with optimized structure
     * 
     * @return Status message indicating success or error reason.
     *         Possible returns: OK with result, ERROR if uninitialized.
     */
    std::string execute() override;
};