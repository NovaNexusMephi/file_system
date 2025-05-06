#pragma once

#include <unordered_set>
#include <vector>

#include "segment.hpp"

namespace filesystem {

/**
 * @brief Enumerates possible error codes returned by Catalog operations.
 */
enum class Error {
    NO_ERROR,             ///< No error occurred.
    NO_FREE_RECORDS,      ///< No available records to allocate.
    NO_FREE_SPACE,        ///< Insufficient free space in the volume.
    FILE_NOT_FOUND,       ///< Requested file does not exist.
    FILE_ALREADY_EXISTS,  ///< File with the same name already exists.
    NO_MEMORY,            ///< Memory allocation failure.
    ERROR_EXCEPTION       ///< Generic/unspecified error.
};

/**
 * @brief Manages file records and storage allocation through segmented structures.
 */
class Catalog final {
   public:
    /**
     * @brief Default constructor. Creates an empty catalog.
     */
    Catalog() = default;

    /**
     * @brief Constructs a catalog with specified configuration.
     * @param count Number of segments to create.
     * @param records_count Maximum records per segment.
     * @param volume_size Total available storage space (bytes).
     */
    Catalog(size_t count, size_t records_count, size_t volume_size)
        : header_(count, 0, records_count * count, volume_size), segments_(count, Segment(records_count)) {}

    /**
     * @brief Prints a visual representation of segment usage to stdout.
     *        Uses [FREE], [BLOCKED], [PERMANENT], and [EMPTY] tags.
     */
    void print_catalog() const noexcept;

    /**
     * @brief Provides read-only access to all segments.
     * @return Constant reference to the vector of segments.
     */
    [[nodiscard]] inline const std::vector<Segment>& get_segments() const noexcept { return segments_; }

    /**
     * @brief Provides mutable access to all segments.
     * @return Reference to the vector of segments.
     */
    [[nodiscard]] inline std::vector<Segment>& get_segments() noexcept { return segments_; }

    /**
     * @brief Gets the number of available file records.
     * @return Reference to the free records counter.
     */
    [[nodiscard]] inline size_t& get_free_records() noexcept { return header_.free_records_; }

    /**
     * @brief Gets the number of available file records (read-only).
     * @return Current count of free records.
     */
    [[nodiscard]] inline size_t get_free_records() const noexcept { return header_.free_records_; }

    /**
     * @brief Gets the remaining free space in the volume.
     * @return Reference to the free space counter (bytes).
     */
    [[nodiscard]] inline size_t& get_free_space() noexcept { return header_.free_space_; }

    /**
     * @brief Gets the remaining free space (read-only).
     * @return Available space in bytes.
     */
    [[nodiscard]] inline size_t get_free_space() const noexcept { return header_.free_space_; }

    /**
     * @brief Gets the number of fully occupied segments.
     * @return Reference to the busy segments counter.
     */
    [[nodiscard]] inline size_t& get_busy_segments_count() noexcept { return header_.counter_; }

    /**
     * @brief Gets the number of fully occupied segments (read-only).
     * @return Count of busy segments.
     */
    [[nodiscard]] inline size_t get_busy_segments_count() const noexcept { return header_.counter_; }

    /**
     * @brief Calculates the number of segments currently in use.
     * @return Total segments used (including partially filled current segment).
     */
    [[nodiscard]] inline size_t get_used_segments_count() const noexcept {
        return header_.counter_ + ((segments_[header_.counter_].get_counter() == 0) ? 0 : 1);
    }

    /**
     * @brief Gets the total number of segments.
     * @return Reference to the segment count.
     */
    [[nodiscard]] inline size_t& get_count() noexcept { return header_.count_; }

    /**
     * @brief Gets the total number of segments (read-only).
     * @return Total segment count.
     */
    [[nodiscard]] inline size_t get_count() const noexcept { return header_.count_; }

    /**
     * @brief Gets the remaining direct allocation space.
     * @return Reference to the free direct space counter (bytes).
     */
    [[nodiscard]] inline size_t& get_free_direct_space() noexcept { return header_.free_direct_space_; }

    /**
     * @brief Gets the remaining direct allocation space (read-only).
     * @return Available direct space in bytes.
     */
    [[nodiscard]] inline size_t get_free_direct_space() const noexcept { return header_.free_direct_space_; }

    /**
     * @brief Gets the space occupied by blocked files.
     * @return Reference to the blocked space counter (bytes).
     */
    [[nodiscard]] inline size_t& get_blocked_space() noexcept { return header_.blocked_space_; }

    /**
     * @brief Gets the space occupied by blocked files (read-only).
     * @return Blocked space in bytes.
     */
    [[nodiscard]] inline size_t get_blocked_space() const noexcept { return header_.blocked_space_; }

    /**
     * @brief Gets the set of tracked filenames.
     * @return Reference to the filename set.
     */
    [[nodiscard]] inline std::unordered_set<std::string>& get_files() noexcept { return files_; }

    /**
     * @brief Gets the set of tracked filenames (read-only).
     * @return Constant reference to the filename set.
     */
    [[nodiscard]] inline const std::unordered_set<std::string>& get_files() const noexcept { return files_; }

    /**
     * @brief Searches for a file record by name.
     * @param filename Name of the file to find.
     * @return Pointer to the FileRecord if found, nullptr otherwise.
     */
    [[nodiscard]] FileRecord* find_record(const std::string& filename) noexcept;

   private:
    /**
     * @brief Contains metadata and counters for catalog management.
     */
    struct CatalogHeader {
        size_t count_;              ///< Total number of segments.
        size_t counter_;            ///< Number of fully occupied segments.
        size_t free_records_;       ///< Available file record slots.
        size_t free_space_;         ///< Total remaining storage space (bytes).
        size_t free_direct_space_;  ///< Space available for direct allocation.
        size_t blocked_space_;      ///< Space occupied by blocked files.

        /**
         * @brief Default constructor. Initializes all counters to zero.
         */
        CatalogHeader() = default;

        /**
         * @brief Constructs a header with specified configuration.
         * @param count Total segments.
         * @param counter Initial busy segments.
         * @param records_count Total file records across all segments.
         * @param volume_size Total storage capacity (bytes).
         */
        CatalogHeader(size_t count, size_t counter, size_t records_count, size_t volume_size)
            : count_(count),
              counter_(counter),
              free_records_(records_count),
              free_space_(volume_size),
              free_direct_space_(volume_size),
              blocked_space_(0) {}
    };

    CatalogHeader header_;                   ///< Catalog metadata and counters.
    std::vector<Segment> segments_;          ///< Collection of storage segments.
    std::unordered_set<std::string> files_;  ///< Set of existing filenames.
};

}  // namespace filesystem