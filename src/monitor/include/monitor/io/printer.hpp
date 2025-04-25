#pragma once

#include <string>
namespace monitor::io {

/**
 * @brief An abstract interface for printing messages.
 */
class Printer {
   public:
    virtual ~Printer() = default;

    /**
     * @brief Prints a message.
     * 
     * @param[in] msg The message to print.
     */
    virtual void print(const std::string& msg) = 0;
};
}  // namespace monitor::io