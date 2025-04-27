#pragma once

#include <string>
namespace monitor::io {

/**
 * @brief Interface for a scanner object that provides methods to iterate over a sequence of elements.
 * 
 * This abstract class defines the contract for classes that implement scanning functionality.
 * It allows iterating over a collection or stream of elements using the `next` and `has_next` methods.
 */
class Scanner {
   public:
    virtual ~Scanner() = default;

    /**
     * @brief Retrieves the next element in the sequence.
     * 
     * This method advances the scanner to the next element and returns it as a string.
     * If there are no more elements, the behavior depends on the implementation (e.g., throwing an exception).
     * 
     * @return std::string The next element in the sequence.
     * 
     * @note It is recommended to call `has_next()` before invoking this method to ensure
     *       that there are more elements available.
     */
    virtual std::string next() = 0;

    /**
     * @brief Checks if there are more elements available in the sequence.
     * 
     * This method determines whether a subsequent call to `next()` will succeed or fail.
     * It is used to safely iterate over the sequence without exceeding its bounds.
     * 
     * @return true If there are more elements available in the sequence.
     * @return false If the end of the sequence has been reached.
     */
    virtual bool has_next() const = 0;
};
}  // namespace monitor::io