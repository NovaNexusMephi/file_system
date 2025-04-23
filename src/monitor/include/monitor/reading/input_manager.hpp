#pragma once

#include <nlohmann/json_fwd.hpp>
#include <optional>
namespace monitor::reading {

/**
 * @brief A utility class responsible for reading and parsing user input commands.
 * 
 * This class provides an interface to read command-line inputs from the user, parse them into a structured JSON format,
 * and return the parsed result. It encapsulates the logic for handling user input and integrates with the `parse_command`
 * function to process the input.
 */
class InputManager final {
   public:
    /**
     * @brief Default constructor.
     * 
     * Initializes an instance of the `InputManager` class. No special initialization logic is required.
     */
    InputManager() = default;

    /**
     * @brief Reads a command from the standard input and parses it into a JSON object.
     * 
     * This function reads a line of input from the user until the delimiter `:` is encountered. The input is then
     * passed to the `parse_command` function for parsing. If the input is invalid or empty, the function returns `std::nullopt`.
     * 
     * @return std::optional<nlohmann::json> A JSON object representing the parsed command, or `std::nullopt` if parsing fails or input is invalid.
     */
    std::optional<nlohmann::json> read_command();
};
}  // namespace monitor::reading