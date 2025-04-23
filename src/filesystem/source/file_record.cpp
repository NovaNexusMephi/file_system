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

[[nodiscard]] std::string filesystem::FileRecord::get_timestamp_as_string() const noexcept {
    std::time_t time = std::chrono::system_clock::to_time_t(timestamp_);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
