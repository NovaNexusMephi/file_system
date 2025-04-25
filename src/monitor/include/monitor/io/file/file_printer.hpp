#pragma once

#include <filesystem>
#include <fstream>
#include "monitor/io/file/file_wrapper.hpp"
#include "monitor/io/printer.hpp"

// TODO: Доделать класс FilePrinter

namespace monitor::io::file {
class FilePrinter final : public monitor::io::Printer, public FileWrapper {
   public:
    FilePrinter() : io::Printer() {}
    explicit FilePrinter(const std::filesystem::path& f_path);
    ~FilePrinter();

    void set_output_file(const std::filesystem::path& f_path);

    void print(const std::string& msg) override;

   private:
    std::ofstream output_file_;
};
}  // namespace monitor::io::file