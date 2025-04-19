#pragma once

#include <chrono>
#include <cstddef>
#include <string>

namespace filesystem {

enum class FileType { TEMPORARY, FREE, PERMANENT, BLOCKED };

class FileRecord final {
   public:
    FileRecord(const std::string& name, size_t size, FileType type = FileType::PERMANENT)
        : type_(type), name_(name), size_(size) {}

    void setFilename(std::string) noexcept;

    void setSize(size_t) noexcept;

    void setType(FileType) noexcept;

    [[nodiscard]] inline std::string getFilename() const noexcept { return name_; }

    [[nodiscard]] inline size_t getSize() const noexcept { return size_; }

    [[nodiscard]] inline FileType getType() const noexcept { return type_; }

    [[nodiscard]] inline std::chrono::system_clock::time_point getTimestamp() const noexcept { return timestamp_; }

   private:
    FileType type_;
    std::string name_;
    size_t size_;
    std::chrono::system_clock::time_point timestamp_;
};

}  // namespace filesystem