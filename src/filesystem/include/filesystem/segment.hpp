#pragma once

#include <cstddef>
#include <vector>

#include "file_record.hpp"

namespace filesystem {

class Segment final {
   public:
    Segment(size_t start, size_t next, size_t size = 0) : header_(start, next) { records_.reserve(size); }

    inline void setNext(size_t next) { header_.setNext(next); }

    inline void setStart(size_t start) { header_.setStart(start); }

    [[nodiscard]] inline size_t getSize() const noexcept { return records_.size(); }

    [[nodiscard]] inline size_t getStart() const noexcept { return header_.getStart(); }

    [[nodiscard]] inline size_t getNext() const noexcept { return header_.getNext(); }

    [[nodiscard]] inline const std::vector<FileRecord>& getRecords() const noexcept { return records_; }

    [[nodiscard]] inline std::vector<FileRecord>& getRecords() noexcept { return records_; }

    bool addRecord(const std::string& filename, size_t size);

   private:
    struct SegmentHeader {
        size_t start_;
        size_t next_;

        SegmentHeader(size_t start, size_t next) : start_(start), next_(next) {}
    };

    SegmentHeader header_;
    std::vector<FileRecord> records_;
};

}  // namespace filesystem