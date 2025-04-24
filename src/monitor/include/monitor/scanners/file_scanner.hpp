#pragma once

#include <filesystem>
#include <fstream>
#include "monitor/core/scanner.hpp"

namespace monitor::scanners {

/**
 * @brief FileScanner reads commands from a file separated by semicolons.
 *        Trims whitespace and ignores empty lines.
 */
class FileScanner final : public core::Scanner {
   public:
    /**
     * @brief Constructs a FileScanner for the specified file path.
     *        Validates file existence and type before opening.
     * @param[in] f_path Path to the input file (absolute or relative)
     * @throws std::invalid_argument if file doesn't exist or isn't regular
     * @throws std::runtime_error if file opening fails
     */
    explicit FileScanner(const std::filesystem::path& f_path);
    ~FileScanner() = default;

    FileScanner(const FileScanner&) = delete;
    FileScanner& operator=(const FileScanner&) = delete;

    FileScanner(FileScanner&&) noexcept = default;
    FileScanner& operator=(FileScanner&&) noexcept = default;

    /**
     * @brief Reads next command from file, split by semicolons.
     *        Trims leading/trailing whitespace. Returns empty string on EOF.
     * @return Trimmed non-empty command, or empty string if no commands remain
     */
    [[nodiscard]] std::string next() override;

    /**
     * @brief Checks if file has remaining commands to read.
     * @return True if file stream is valid and has unread data, else false
     */
    [[nodiscard]] bool has_next() const override;

   private:
    mutable std::ifstream file_;
};
}  // namespace monitor::scanners