#include "filesystem/file_record.hpp"

void filesystem::FileRecord::set_filename(std::string name) noexcept {
    name_ = std::move(name);
}

void filesystem::FileRecord::set_size(size_t size) noexcept {
    size_ = size;
}

void filesystem::FileRecord::set_type(FileType type) noexcept {
    type_ = type;
}
