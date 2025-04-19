#pragma once

#include <chrono>
#include <cstddef>
#include <string>

namespace filesystem {

enum class FileType { TEMPORARY, FREE, PERMANENT, BLOCKED };

class FileRecord {
   public:
    FileRecord(const std::string& filename, const std::string& content);
    std::string getFilename() const;
    std::string getContent() const;

   private:
    FileType type_;
    std::string name_;
    size_t size_;
    std::chrono::system_clock::time_point timestamp_;
};

}  // namespace filesystem