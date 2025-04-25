#pragma once

#include "monitor/io/io_factory.hpp"
#include "monitor/io/scanner.hpp"

#include <filesystem>
#include <memory>
namespace fs = std::filesystem;

// TODO: Доделать фабрику FileFactory

namespace monitor::io::file {
class FileFactory final : public IOFactory {
   public:
    FileFactory() = default;
    FileFactory(const fs::path& input_file, const fs::path& output_dir);

    void set_input_file(const std::filesystem::path& input_file);
    void set_output_dir(const std::filesystem::path& output_dir);

    [[nodiscard]] std::unique_ptr<Printer> create_printer(const std::string& p_name) const override;
    [[nodiscard]] std::unique_ptr<Scanner> create_scanner(const std::string& s_name) const override;

   private:
    std::filesystem::path input_fpath_{};
    std::filesystem::path output_dpath_{};

    static fs::path validate_file(const fs::path& f_path);
    static fs::path validate_directory(const fs::path& dir_path);
};
}  // namespace monitor::io::file