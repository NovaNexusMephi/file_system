#pragma once

#include "catalog.hpp"
#include "system_information.hpp"

namespace filesystem {

/**
 * @brief Manages the overall file system including system information and catalog.
 */
class FileSystem {
   public:
    /**
     * @brief Default constructor. Initializes an invalid file system.
     */
    FileSystem() = default;

    /**
     * @brief Retrieves the system information block (read-only).
     * @return Constant reference to the SystemInformation object.
     */
    [[nodiscard]] inline const SystemInformation& get_info() const noexcept { return info_block_; }

    /**
     * @brief Retrieves the system information block (mutable).
     * @return Reference to the SystemInformation object.
     */
    [[nodiscard]] inline SystemInformation& get_info() noexcept { return info_block_; }

    /**
     * @brief Updates the system information block.
     * @param info The new SystemInformation to assign.
     */
    inline void set_info(const SystemInformation& info) noexcept { info_block_ = info; }

    /**
     * @brief Retrieves the file catalog (read-only).
     * @return Constant reference to the Catalog object.
     */
    [[nodiscard]] inline const Catalog& get_catalog() const noexcept { return catalog_; }

    /**
     * @brief Retrieves the file catalog (mutable).
     * @return Reference to the Catalog object.
     */
    [[nodiscard]] inline Catalog& get_catalog() noexcept { return catalog_; }

    /**
     * @brief Checks if the file system is in a valid state.
     * @return True if valid, false otherwise.
     */
    [[nodiscard]] inline bool is_valid() const noexcept { return valid_; }

    /**
     * @brief Marks the file system as valid.
     */
    inline void validate() noexcept { valid_ = true; }

    /**
     * @brief Marks the file system as invalid.
     */
    inline void invalidate() noexcept { valid_ = false; }

    /**
     * @brief Initializes the file catalog with specified configuration.
     * @param segm Number of segments to allocate in the catalog.
     * @param rec Maximum number of records per segment.
     * @param vol Total volume size (in bytes) managed by the file system.
     */
    inline void init_catalog(size_t segm, size_t rec, size_t vol) noexcept { catalog_ = Catalog(segm, rec, vol); }

   private:
    SystemInformation info_block_;  ///< System metadata and configuration.
    Catalog catalog_;               ///< Manages file records and segments.
    bool valid_ = false;            ///< Tracks if the file system is initialized and operational.
};

}  // namespace filesystem