#include "filesystem/segment.hpp"

#include <iostream>

#include "filesystem/file_record.hpp"

namespace filesystem {

bool Segment::add_record(const std::string& filename, size_t size) {
    records_.push_back(FileRecord(filename, size));
    ++header_.counter_;
    if (header_.counter_ == header_.size_) {
        return true;
    }
    return false;
}

}  // namespace filesystem
