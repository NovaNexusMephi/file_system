#pragma once

#include <string>
namespace monitor::io {
/**
 * @brief An abstract class for reading input.
 */
class Scanner {
   public:
    Scanner() = default;
    explicit Scanner(const std::string& s_name);

    virtual ~Scanner() = default;

    [[nodiscard]] std::string get_name() const noexcept;

    void set_name(const std::string& name);

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

   protected:
    std::string name_;
};
}  // namespace monitor::io