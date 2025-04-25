#pragma once

#include "monitor/io/scanner.hpp"

namespace monitor::io::console {

/**
 * @brief A console-based implementation of the Scanner interface.
 * 
 * This class reads input from the console, processes it by trimming whitespace,
 * and splits it into commands based on the ';' delimiter. It also provides a method
 * to check if more input is available.
 */
class ConsoleScanner final : public io::Scanner {
   public:
    ~ConsoleScanner() override = default;

    /**
     * @brief Reads the next trimmed input command from the console.
     * 
     * This method reads input line-by-line from the console until a non-empty
     * command is found. Each line is split by the ';' delimiter, and leading/trailing
     * whitespace is removed from each segment. If no non-empty command is found,
     * an empty string is returned.
     * 
     * @return std::string The next trimmed input command, or an empty string if no command is available.
     */
    [[nodiscard]] std::string next() override;

    /**
     * @brief Checks if there is more input available in the console.
     * 
     * This method determines if the input stream is still valid and has more data to read.
     * It checks for end-of-file (EOF), stream errors, and whether the next character is not EOF.
     * 
     * @return true If there is more input available.
     * @return false If the input stream has reached EOF or encountered an error.
     */
    [[nodiscard]] bool has_next() const override;
};
}  // namespace monitor::io::console