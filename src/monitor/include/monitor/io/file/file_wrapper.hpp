#pragma once

#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

namespace monitor::io::file {
class FileWrapper {
   public:
    FileWrapper(const fs::path& f_path, std::ios::openmode mode);
    virtual ~FileWrapper() = default;

   protected:
    std::fstream stream_;
    fs::path path_;
    std::ios::openmode mode_;
};
}  // namespace monitor::io::file