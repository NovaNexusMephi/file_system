#include "filesystem/file_record.hpp"

void filesystem::FileRecord::setFilename(std::string name) noexcept {
    name_ = std::move(name);
}

void filesystem::FileRecord::setSize(size_t size) noexcept {
    size_ = size;
}

void filesystem::FileRecord::setType(FileType type) noexcept {
    type_ = type;
}
