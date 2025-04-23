#pragma once

#include <cstddef>
#include <vector>

#include "file_record.hpp"

namespace filesystem {

class Segment final {
   public:
    Segment(size_t start, size_t size = 0) : header_(start, 0, size) {}

    inline void set_start(size_t start) noexcept { header_.start_ = start; }

    [[nodiscard]] inline size_t get_size() const noexcept { return records_.size(); }

    [[nodiscard]] inline size_t get_start() const noexcept { return header_.start_; }

    [[nodiscard]] inline const std::vector<FileRecord>& get_records() const noexcept { return records_; }

    [[nodiscard]] inline std::vector<FileRecord>& get_records() noexcept { return records_; }

    [[nodiscard]] inline size_t get_counter() const noexcept { return header_.counter_; }

    bool add_record(const std::string& filename, size_t size) noexcept;

   private:
    struct SegmentHeader {
        size_t start_;
        size_t counter_;
        size_t size_;

        SegmentHeader(size_t start, size_t counter, size_t size) : start_(start), counter_(counter), size_(size) {}
    };

    SegmentHeader header_;
    std::vector<FileRecord> records_;
};

}  // namespace filesystem