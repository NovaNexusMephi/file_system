#pragma once

#include <istream>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <utility>

namespace monitor {
/**
 * @class Reader
 * @brief A utility class for parsing commands from an input stream into JSON objects.
 *
 * The `Reader` class is designed to process a stream of commands separated by semicolons (`;`).
 * Each command is parsed into a JSON object containing the command name and its associated options.
 * Options can be flags (without values), numeric values, or strings. The class ensures proper
 * handling of whitespace, empty values, and malformed inputs.
 */
class Reader final {
   public:
    Reader() = delete;

    /**
      * @brief Constructs a `Reader` object with an input stream.
      * @param[in] stream The input stream from which commands will be read.
      */
    explicit Reader(std::istream& stream) : stream_(stream) {}

    /**
      * @brief Reads and parses the next command from the input stream.
      * @return An optional JSON object representing the parsed command, or `std::nullopt`
      *         if no valid command is available.
      *
      * This method reads the next command from the input stream, trims unnecessary whitespace,
      * and parses it into a JSON object. The JSON object contains:
      * - A `command_name` field with the name of the command.
      * - An `options` field with key-value pairs representing the command's options.
      *
      * If the input stream is empty or contains invalid data, the method returns `std::nullopt`.
      *
      * ### Example:
      * Input: `"init \\rec:2 \\vol:10;"`
      * Output:
      * ```json
      * {
      *     "command_name": "init",
      *     "options": {
      *         "rec": 2,
      *         "vol": 10
      *     }
      * }
      * ```
      */
    [[nodiscard]] std::optional<nlohmann::json> read();

   private:
    /**
      * @brief Removes leading and trailing whitespace from a string.
      * @param[in] str The input string to trim.
      * @return The trimmed string with whitespace removed from both ends.
      */
    std::string trim(const std::string& str);

    /**
      * @brief Splits a string into a key-value pair based on the `:` delimiter.
      * @param[in] input The input string to split (expected to start with `\`).
      * @return A pair consisting of the key and an optional value. If the input is invalid,
      *         the key will be an empty string, and the value will be `std::nullopt`.
      *
      * This private helper method processes strings in the format `\\key:value` or `\\flag`.
      * - If the string contains a `:` delimiter, it splits the string into a key and value.
      * - If the string does not contain a `:` delimiter, it treats the string as a flag
      *   with no value.
      * - If the input is malformed (e.g., does not start with `\`), the method returns
      *   an empty key and `std::nullopt`.
      */
    std::pair<std::string, std::optional<std::string>> split_val(const std::string& input);

    /**
      * @brief The input stream from which commands are read.
      */
    std::istream& stream_;
};
}  // namespace monitor