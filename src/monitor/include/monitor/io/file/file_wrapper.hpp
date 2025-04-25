#pragma once

#include <filesystem>
namespace fs = std::filesystem;

namespace monitor::io::file {
class FileWrapper {
   public:
    virtual ~FileWrapper() = default;

   protected:
    [[nodiscard]] fs::path validate(const fs::path& path);
};
}  // namespace monitor::io::file