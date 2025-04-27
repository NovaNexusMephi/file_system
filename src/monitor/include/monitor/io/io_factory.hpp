#pragma once

#include <memory>
#include "monitor/io/printer.hpp"
#include "monitor/io/scanner.hpp"

namespace monitor::io {
/**
 * @brief Interface for a factory that creates Scanner and Printer objects.
 * 
 * This abstract class defines the contract for creating instances of `Scanner` and `Printer`.
 * It allows derived classes to implement specific creation logic, enabling flexible and decoupled object creation.
 */
class IOFactory {
   public:
    virtual ~IOFactory() = default;

    /**
     * @brief Creates and returns a new Scanner object.
     * 
     * This method is responsible for instantiating a `Scanner` object using the implementation
     * provided by the derived class. The returned object is wrapped in a `std::unique_ptr`
     * to ensure proper ownership and lifetime management.
     * 
     * @return std::unique_ptr<Scanner> A unique pointer to the newly created Scanner object.
     */
    virtual std::unique_ptr<Scanner> create_scanner() const noexcept = 0;

    /**
     * @brief Creates and returns a new Printer object.
     * 
     * This method is responsible for instantiating a `Printer` object using the implementation
     * provided by the derived class. The returned object is wrapped in a `std::unique_ptr`
     * to ensure proper ownership and lifetime management.
     * 
     * @return std::unique_ptr<Printer> A unique pointer to the newly created Printer object.
     */
    virtual std::unique_ptr<Printer> create_printer() const noexcept = 0;
};
}  // namespace monitor::io