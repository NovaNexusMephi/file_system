#pragma once

#include <string>
namespace monitor::io {
/**
 * @brief Interface for a printer object that provides functionality to output messages.
 * 
 * This abstract class defines the contract for classes that implement printing functionality.
 * It allows derived classes to define how messages are printed, such as to the console, a file, or another output stream.
 */
class Printer {
   public:
    virtual ~Printer() = default;

    /**
     * @brief Prints the given message.
     * 
     * This method outputs the provided message using the implementation defined by the derived class.
     * The exact behavior (e.g., destination of the output) depends on the specific implementation.
     * 
     * @param[in] msg The message to be printed. Must be a valid string.
     */
    virtual void print(const std::string& msg) = 0;
};
}  // namespace monitor::io