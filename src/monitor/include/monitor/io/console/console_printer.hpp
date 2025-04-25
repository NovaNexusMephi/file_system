#pragma once

#include "monitor/io/printer.hpp"

namespace monitor::io::console {

/**
 * @brief A console-based implementation of the Printer interface.
 */
class ConsolePrinter final : public io::Printer {
   public:
    ~ConsolePrinter() = default;

    /**
     * @brief Prints a message to the console.
     * 
     * @param[in] msg The message to print.
     */
    void print(const std::string& msg) override;
};
}  // namespace monitor::io::console