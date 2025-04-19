#pragma once

#include <cstddef>
#include <vector>

#include "file_record.hpp"

namespace filesystem {

class Segment final {
public:
    
    Segment(size_t start, size_t next, size_t size = 0) : header_(start, next) {
        records_.reserve(size);
    }

    inline void setNext(size_t next) {
        header_.setNext(next);
    }

    [[nodiscard]] inline const std::vector<FileRecord>& getRecords() const noexcept {
        return records_;
    }

    [[nodiscard]] inline std::vector<FileRecord>& getRecords() noexcept {
        return records_;
    }

    [[nodiscard]] inline size_t getSize() const noexcept {
        return records_.size();
    }

private:
    struct SegmentHeader {
        size_t start_;
        size_t next_;

        SegmentHeader(size_t start, size_t next) : start_(start), next_(next) {}

        inline void setNext(size_t next) {
            next_ = next;
        }
    };

    SegmentHeader header_;
    std::vector<FileRecord> records_;
};

}  // namespace filesystem