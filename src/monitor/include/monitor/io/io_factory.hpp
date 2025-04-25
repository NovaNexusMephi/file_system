#pragma once

#include <memory>
#include "monitor/io/printer.hpp"
#include "monitor/io/scanner.hpp"

namespace monitor::io {

/**
 * @brief An abstract factory interface for creating Printer and Scanner objects.
 */
class IOFactory {
   public:
    virtual ~IOFactory() = default;

    /**
     * @brief Creates a Printer object.
     * 
     * @return std::unique_ptr<Printer> A unique pointer to a Printer instance.
     */
    virtual std::unique_ptr<Printer> create_printer() const = 0;

    /**
     * @brief Creates a Scanner object.
     * 
     * @return std::unique_ptr<Scanner> A unique pointer to a Scanner instance.
     */
    virtual std::unique_ptr<Scanner> create_scanner() const = 0;
};
}  // namespace monitor::io