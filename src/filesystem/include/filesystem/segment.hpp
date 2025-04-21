#pragma once

#include <cstddef>
#include <vector>

#include "file_record.hpp"

namespace filesystem {

class Segment final {
   public:
    Segment(size_t start, size_t next, size_t size = 0) : header_(start, next) { records_.reserve(size); }

    inline void set_next(size_t next) noexcept { 
        header_.next_ = next; 
    }

    inline void setStart(size_t start) noexcept { 
        header_.start_ = start; 
    }

    [[nodiscard]] inline size_t get_size() const noexcept { 
        return records_.size(); 
    }

    [[nodiscard]] inline size_t get_start() const noexcept { 
        return header_.start_; 
    }

    [[nodiscard]] inline size_t get_next() const noexcept { 
        return header_.next_; 
    }

    [[nodiscard]] inline const std::vector<FileRecord>& get_records() const noexcept { 
        return records_; 
    }

    [[nodiscard]] inline std::vector<FileRecord>& get_records() noexcept { 
        return records_; 
    }

    bool add_record(const std::string& filename, size_t size);

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