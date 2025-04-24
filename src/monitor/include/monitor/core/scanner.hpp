#pragma once

#include <string>
namespace monitor::core {
class ScanManager;

/**
 * @brief Interface for command scanners that read input sources split by semicolons.
 *        Implementations handle trimming whitespace and skipping empty lines.
 */
class Scanner {
   public:
    virtual ~Scanner() = default;

    /**
     * @brief Retrieves the next command from the input source.
     *        Commands are split by semicolons, trimmed of whitespace,
     *        and empty lines are skipped.
     * @return Trimmed non-empty command, or empty string if no commands remain
     */
    virtual std::string next() = 0;

    /**
     * @brief Checks if there are remaining commands to read.
     * @return True if unread commands exist in the input source, else false
     */
    virtual bool has_next() const = 0;
};
}  // namespace monitor::core