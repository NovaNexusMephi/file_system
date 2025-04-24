#pragma once

#include "monitor/core/scanner.hpp"

namespace monitor::scanners {
/**
 * @brief ConsoleScanner reads commands from stdin separated by semicolons.
 *        Trims whitespace and ignores empty lines.
 */
class ConsoleScanner final : public core::Scanner {
   public:
    ConsoleScanner() = default;
    ~ConsoleScanner() override = default;

    ConsoleScanner(const ConsoleScanner&) = delete;
    ConsoleScanner& operator=(ConsoleScanner&) = delete;

    ConsoleScanner(ConsoleScanner&&) noexcept = default;
    ConsoleScanner& operator=(ConsoleScanner&&) noexcept = default;

    /**
     * @brief Reads next command from input, split by semicolons.
     *        Trims leading/trailing whitespace. Returns empty string on EOF.
     * @return Trimmed non-empty command, or empty string if no more commands.
     */
    [[nodiscard]] std::string next() override;

    /**
     * @brief Checks if input has remaining commands.
     * @return True if input stream is valid and has unread data, false otherwise.
     */
    [[nodiscard]] bool has_next() const override;
};
}  // namespace monitor::scanners