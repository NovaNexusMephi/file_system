#pragma once

#include <filesystem>
#include <fstream>
#include "monitor/io/scanner.hpp"

// TODO: Доделать класс Scanner

namespace monitor::io::file {
class FileScanner final : public monitor::io::Scanner {
   public:
    FileScanner() = default;
    explicit FileScanner(const std::filesystem::path& input_file);
    ~FileScanner();

    void set_input_file(const std::filesystem::path& f_path);

    [[nodiscard]] std::string next() override;
    [[nodiscard]] bool has_next() const override;

   private:
    std::ifstream input_file_;
};
}  // namespace monitor::io::file