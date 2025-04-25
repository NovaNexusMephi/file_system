#pragma once

#include <string>
namespace monitor::io {
/**
 * @brief An abstract interface for reading input.
 */
class Scanner {
   public:
    virtual ~Scanner() = default;

    /**
     * @brief Reads the next input.
     * 
     * @return std::string The next input string.
     */
    virtual std::string next() = 0;

    /**
     * @brief Checks if there is more input available.
     * 
     * @return true If there is more input available.
     * @return false If there is no more input available.
     */
    virtual bool has_next() const = 0;
};
}  // namespace monitor::io