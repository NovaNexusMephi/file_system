#pragma once

#include <optional>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace monitor {
/**
 * @brief A utility class for parsing command-line inputs into structured JSON objects.
 * 
 * The CommandParser class provides static methods to parse a command-line string, split it into tokens,
 * and organize the tokens into a JSON object representing the command name, arguments, and options.
 */
class Parser {
   public:
    /**
     * @brief Parses a command-line string into a structured JSON object.
     * 
     * This method splits the input command-line string into tokens, categorizes them into arguments and options,
     * and constructs a JSON object containing the command name, its parameters, and associated options.
     * 
     * @param[in] cmd_line The command-line string to be parsed.
     * @return std::optional<nlohmann::json> A JSON object representing the parsed command, or std::nullopt if parsing fails.
     */
    [[nodiscard]] static std::optional<nlohmann::json> parse(const std::string& cmd_line) noexcept;

   private:
    /**
     * @brief Splits a string into tokens based on specified delimiters.
     * 
     * This method tokenizes the input string by splitting it at each occurrence of any delimiter character.
     * Consecutive delimiters are treated as a single delimiter, and empty tokens are ignored.
     * 
     * @param[in] str The input string to be split.
     * @param[in] delims A string containing delimiter characters.
     * @return std::vector<std::string> A vector of tokens extracted from the input string.
     */
    [[nodiscard]] static std::vector<std::string> split(const std::string& str, const std::string& delims) noexcept;

    /**
     * @brief Parses an option token into a key-value pair.
     * 
     * This method processes an option token (starting with '/') and extracts the key and value. If the token contains
     * a colon (':'), the part before the colon is treated as the key, and the part after is treated as the value.
     * If no colon is present, the entire token (excluding the leading '/') is treated as the key, and the value is empty.
     * 
     * @param[in] opt_token The option token to be parsed.
     * @return std::pair<std::string, std::string> A pair containing the key and value extracted from the token.
     */
    [[nodiscard]] static std::pair<std::string, std::string> parse_opt(const std::string& opt_token) noexcept;

    /**
     * @brief Constructs a JSON object from a list of option tokens.
     * 
     * This method processes a list of option tokens, parses each token into a key-value pair, and constructs a JSON object.
     * Numeric values are converted to integers, while other values remain as strings. If no value is provided for an option,
     * it is set to `true` in the JSON object.
     * 
     * @param[in] opt_tokens A vector of option tokens to be processed.
     * @return nlohmann::json A JSON object representing the parsed options.
     */
    [[nodiscard]] static nlohmann::json make_opt_obj(const std::vector<std::string>& opt_tokens) noexcept;

    /**
     * @brief Constructs a command name and its parameter list from argument tokens.
     * 
     * This method takes a list of argument tokens, treats the first token as the command name, and the remaining tokens
     * as parameters. It returns the command name and the list of parameters as a pair.
     * 
     * @param[in] arg_tokens A vector of argument tokens to be processed.
     * @return std::pair<std::string, std::vector<std::string>> A pair containing the command name and its parameters.
     */
    [[nodiscard]] static std::pair<std::string, std::vector<std::string>> make_argv(
        const std::vector<std::string>& arg_tokens) noexcept;
};
}  // namespace monitor