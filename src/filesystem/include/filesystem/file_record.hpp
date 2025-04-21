#pragma once

#include <chrono>
#include <cstddef>
#include <string>

namespace filesystem {

enum class FileType { FREE, PERMANENT, BLOCKED };

class FileRecord final {
   public:
    FileRecord(const std::string& name, size_t size, FileType type = FileType::PERMANENT)
        : type_(type), name_(name), size_(size) {}

    void set_filename(std::string) noexcept;

    void set_size(size_t) noexcept;

    void set_type(FileType) noexcept;

    [[nodiscard]] inline std::string get_filename() const noexcept { return name_; }

    [[nodiscard]] inline size_t get_size() const noexcept { return size_; }

    [[nodiscard]] inline FileType get_type() const noexcept { return type_; }

    [[nodiscard]] inline std::chrono::system_clock::time_point get_timestamp() const noexcept { return timestamp_; }

   private:
    FileType type_;
    std::string name_;
    size_t size_;
    std::chrono::system_clock::time_point timestamp_;
};

}  // namespace filesystem