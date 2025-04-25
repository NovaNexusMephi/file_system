#pragma once

#include "monitor/io/io_factory.hpp"

namespace monitor::io::console {
/**
 * @brief A factory class for creating console-based Printer and Scanner objects.
 */
class ConsoleFactory final : public IOFactory {
   public:
    ~ConsoleFactory() override = default;

    /**
     * @brief Creates a console-based Printer object.
     * 
     * @return std::unique_ptr<Printer> A unique pointer to a Printer instance.
     */
    std::unique_ptr<Printer> create_printer(const std::string& p_name) const override;

    /**
     * @brief Creates a console-based Scanner object.
     * 
     * @return std::unique_ptr<Scanner> A unique pointer to a Scanner instance.
     */
    std::unique_ptr<Scanner> create_scanner(const std::string& s_name) const override;
};
}  // namespace monitor::io::console