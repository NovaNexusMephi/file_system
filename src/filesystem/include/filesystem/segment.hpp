#pragma once

#include <cstddef>
#include <vector>

#include "file_record.hpp"

namespace filesystem {

/**
 * @brief Represents a fixed-capacity segment for managing a collection of FileRecords.
 */
class Segment final {
   public:
    /**
      * @brief Constructs a Segment with a specified maximum capacity.
      * @param size The maximum number of FileRecords the segment can hold.
      */
    Segment(size_t size) : header_(size) {}

    /**
      * @brief Retrieves the maximum capacity of the segment.
      * @return The total number of FileRecords the segment can hold.
      */
    [[nodiscard]] inline size_t get_size() const noexcept { return header_.size_; }

    /**
      * @brief Provides read-only access to the list of FileRecords in the segment.
      * @return A constant reference to the vector of FileRecords.
      */
    [[nodiscard]] inline const std::vector<FileRecord>& get_records() const noexcept { return records_; }

    /**
      * @brief Provides mutable access to the list of FileRecords in the segment.
      * @return A reference to the vector of FileRecords.
      */
    [[nodiscard]] inline std::vector<FileRecord>& get_records() noexcept { return records_; }

    /**
      * @brief Retrieves the current number of FileRecords in the segment.
      * @return The current count of FileRecords.
      */
    [[nodiscard]] inline size_t get_counter() const noexcept { return header_.counter_; }

    /**
      * @brief Adds a new FileRecord to the segment.
      * @param filename The name of the file to add.
      * @param size The size of the file in bytes.
      * @return True if the segment is full after adding, false otherwise.
      */
    [[nodiscard]] bool add_record(const std::string& filename, size_t size) noexcept;

    /**
      * @brief Removes the last FileRecord from the segment.
      * @note Decrements the internal counter and modifies the records vector.
      */
    void remove_record() noexcept;

   private:
    /**
      * @brief Internal structure to track segment metadata.
      */
    struct SegmentHeader {
        size_t size_;     ///< Maximum capacity of the segment.
        size_t counter_;  ///< Current number of records in the segment.

        /**
          * @brief Constructs a SegmentHeader with the given capacity.
          * @param size The maximum number of FileRecords allowed.
          */
        SegmentHeader(size_t size) : size_(size), counter_(0) {}
    };

    SegmentHeader header_;             ///< Metadata for the segment.
    std::vector<FileRecord> records_;  ///< Collection of FileRecords managed by the segment.
};

}  // namespace filesystem