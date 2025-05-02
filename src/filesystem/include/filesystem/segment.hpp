#pragma once

#include <cstddef>
#include <vector>

#include "file_record.hpp"

namespace filesystem {

class Segment final {
   public:
    Segment(size_t size) : header_(size) {}

    [[nodiscard]] inline size_t get_size() const noexcept { return header_.size_; }

    [[nodiscard]] inline const std::vector<FileRecord>& get_records() const noexcept { return records_; }

    [[nodiscard]] inline std::vector<FileRecord>& get_records() noexcept { return records_; }

    [[nodiscard]] inline size_t get_counter() const noexcept { return header_.counter_; }

    [[nodiscard]] bool add_record(const std::string& filename, size_t size) noexcept;

    void remove_record() noexcept;

   private:
    struct SegmentHeader {
        size_t size_;
        size_t counter_;

        SegmentHeader(size_t size) : size_(size), counter_(0) {}
    };

    SegmentHeader header_;
    std::vector<FileRecord> records_;
};

}  // namespace filesystem