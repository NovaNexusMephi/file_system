#pragma once

#include "abstract_command.hpp"

/**
 * @brief Command to sort and display file system entries based on specified criteria.
 * 
 * Supports sorting by name, extension, date, or size with optional reverse order.
 * Follows the Command design pattern for operation consistency.
 */
class SortCommand : public AbstractCommand {
   public:
    /**
     * @brief Constructs a SortCommand with sorting parameters.
     * 
     * @param file Reference to the target FileSystem instance.
     * @param sort_by Sorting criterion: "name", "ext", "date", or "size".
     *                Defaults to "name".
     * @param inv If "inv", results are reversed after sorting.
     */
    explicit SortCommand(filesystem::FileSystem& file, const std::string& sort_by = "", const std::string& inv = "")
        : AbstractCommand(file), sort_by_(sort_by), inv_(inv) {}

    /**
     * @brief Executes the sorting and display operation.
     * 
     * Process steps:
     * 1. Validate file system initialization
     * 2. Collect all active file records
     * 3. Sort using the specified criteria
     * 4. Reverse order if inversion requested
     * 5. Format results into standardized output
     * 
     * @return Sorted file listing or error message.
     *         Possible returns: OK with sorted list, ERROR if uninitialized.
     */
    std::string execute() override;

   private:
    std::string sort_by_;  ///< Sorting criterion (name/ext/date/size)
    std::string inv_;      ///< Inversion flag ("inv" for reverse order)
};