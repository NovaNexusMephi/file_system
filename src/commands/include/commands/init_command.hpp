#pragma once

#include <string>
#include "abstract_command.hpp"

/**
 * @brief Command to initialize the file system with specified parameters.
 * 
 * Handles file system formatting by configuring storage parameters and
 * preparing the catalog structure. Follows the Command design pattern.
 */
class InitCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs an InitCommand with file system configuration.
     * 
     * @param file Reference to the target FileSystem instance.
     * @param v Volume label/name for the file system.
     * @param ow Owner name (optional).
     * @param segm Number of segments in the catalog.
     * @param rec Maximum records per segment.
     * @param vol Total volume size in bytes.
     */
    explicit InitCommand(filesystem::FileSystem& file, const std::string& v, const std::string& ow, size_t segm,
                         size_t rec, size_t vol)
        : AbstractCommand(file), volume_(v), owner_(ow), segm_(segm), vol_(vol), rec_(rec) {};

    /**
     * @brief Executes the file system initialization.
     * 
     * The process includes:
     * 1. Check if already initialized
     * 2. Set system metadata (volume/owner)
     * 3. Initialize catalog structure
     * 4. Mark file system as valid
     * 
     * @return OK on success, ERROR if already initialized.
     */
    std::string execute() override;

   private:
    std::string volume_;  ///< Volume label for the file system
    std::string owner_;   ///< Owner name (optional)
    size_t segm_;         ///< Number of catalog segments
    size_t vol_;          ///< Total volume size in bytes
    size_t rec_;          ///< Maximum file records per segment
};