#include "monitor/scanners/file_scanner.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
using monitor::scanners::FileScanner;

FileScanner::FileScanner(const std::filesystem::path& f_path) {
    const auto abs_path = std::filesystem::absolute(f_path);

    if (!std::filesystem::exists(abs_path)) {
        throw std::invalid_argument("File not found: " + abs_path.string());
    }

    if (!std::filesystem::is_regular_file(abs_path)) {
        throw std::invalid_argument("Not a regular file: " + abs_path.string());
    }

    file_.open(abs_path, std::ios::in);
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open file: " + abs_path.string());
    }
}

[[nodiscard]] std::string FileScanner::next() {
    std::string line;
    while (std::getline(file_, line, ';')) {
        line.erase(line.begin(),
                   std::find_if(line.begin(), line.end(), [](unsigned char c) { return !std::isspace(c); }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char c) { return !std::isspace(c); }).base(),
                   line.end());
        if (!line.empty()) {
            return line;
        }
    }
    return "";
}

[[nodiscard]] bool FileScanner::has_next() const {
    return file_ && !file_.eof() && (file_.peek() != std::ifstream::traits_type::eof());
}