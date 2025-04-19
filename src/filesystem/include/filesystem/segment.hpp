#pragma once

#include <cstddef>
#include <vector>

#include "file_record.hpp"

namespace filesystem {

class Segment {
   public:
    Segment(size_t size);
    size_t getSize() const;

   private:
    struct SegmentHeader {
        size_t start_;
        size_t next_;
    };

    SegmentHeader header_;
    std::vector<FileRecord> records_;
};

}  // namespace filesystem