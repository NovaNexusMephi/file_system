#include "monitor/io/file/file_factory.hpp"
#include <filesystem>
#include <memory>
#include "monitor/io/file/file_printer.hpp"
#include "monitor/io/file/file_scanner.hpp"
#include "monitor/io/printer.hpp"
#include "monitor/io/scanner.hpp"
using monitor::io::Printer;
using monitor::io::Scanner;
using monitor::io::file::FileFactory;

FileFactory::FileFactory(const fs::path& input_file, const fs::path& output_dir) {
    set_input_file(input_file);
    set_output_dir(output_dir);
}

void FileFactory::set_input_file(const fs::path& new_path) {
    auto old_input_path = input_fpath_;
    try {
        input_fpath_ = validate_file(new_path);
    } catch (...) {
        input_fpath_ = old_input_path;
        throw;
    }
}

void FileFactory::set_output_dir(const fs::path& output_dpath) {
    auto old_output_dpath = output_dpath_;
    try {
        output_dpath_ = validate_directory(output_dpath);
    } catch (...) {
        output_dpath_ = old_output_dpath;
        throw;
    }
}

std::unique_ptr<Printer> FileFactory::create_printer(const std::string& p_name) const {
    return std::make_unique<FilePrinter>(output_dpath_);
}

std::unique_ptr<Scanner> FileFactory::create_scanner(const std::string& s_name) const {
    return std::make_unique<FileScanner>(input_fpath_);
}