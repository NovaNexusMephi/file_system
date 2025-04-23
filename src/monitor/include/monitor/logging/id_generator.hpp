#pragma once

#include <string>
namespace monitor::logging {
/**
 * @brief A utility class responsible for generating unique identifiers (IDs).
 * 
 * This class provides a static method to generate unique IDs based on the current timestamp and random data.
 * The generated IDs are formatted as strings, combining the date, time, and a random number.
 */
class IDGenerator final {
   public:
    /**
     * @brief Generates a unique identifier based on the current timestamp and random data.
     * 
     * This method creates a unique ID by combining the current date and time (in the format `YYYYMMDDHHMMSS`)
     * with a 4-digit random number. The result is returned as a string.
     * 
     * @return std::string A string representation of the generated unique ID.
     */
    [[nodiscard]] static std::string generate() noexcept;
};
}  // namespace monitor::logging