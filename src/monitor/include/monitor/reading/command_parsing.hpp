#pragma once

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace monitor::reading {

/**
 * @brief Parses a command line string into a structured JSON object.
 * 
 * This function splits the input command line into tokens, separates arguments and options,
 * and constructs a JSON object containing the command name, parameters, and options.
 * 
 * @param[in] cmd_line The input command line string to parse.
 * @return std::optional<nlohmann::json> A JSON object representing the parsed command, or `std::nullopt` if parsing fails.
 */
[[nodiscard]] std::optional<nlohmann::json> parse_command(const std::string& cmd_line) noexcept;

/**
 * @brief Splits a string into tokens based on a set of delimiters.
 * 
 * This function tokenizes the input string by splitting it at any character present in the delimiter string.
 * Empty tokens are ignored.
 * 
 * @param[in] str The input string to split.
 * @param[in] delims A string containing delimiter characters.
 * @return std::vector<std::string> A vector of tokens extracted from the input string.
 */
[[nodiscard]] std::vector<std::string> split(const std::string& str, const std::string& delims) noexcept;

/**
 * @brief Converts an option token into a key-value pair.
 * 
 * This function processes an option token (e.g., `/key:value`) and extracts the key and value.
 * If the token does not contain a colon (`:`), the value is considered empty.
 * 
 * @param[in] opt_token The option token to process.
 * @return std::pair<std::string, std::string> A pair containing the key and value extracted from the token.
 */
[[nodiscard]] std::pair<std::string, std::string> make_opt(const std::string& opt_token) noexcept;

/**
 * @brief Constructs a JSON object from a list of option tokens.
 * 
 * This function processes a list of option tokens, converts them into key-value pairs using `make_opt`,
 * and constructs a JSON object where keys map to their corresponding values.
 * 
 * @param[in] opt_tokens A vector of option tokens to process.
 * @return nlohmann::json A JSON object representing the options.
 */
[[nodiscard]] nlohmann::json make_opt_obj(const std::vector<std::string>& opt_tokens) noexcept;

/**
 * @brief Extracts the command name and parameters from a list of argument tokens.
 * 
 * This function separates the first token as the command name and treats the remaining tokens as parameters.
 * 
 * @param[in] arg_tokens A vector of argument tokens.
 * @return std::pair<std::string, std::vector<std::string>> A pair containing the command name and a vector of parameters.
 */
[[nodiscard]] std::pair<std::string, std::vector<std::string>> name_params(
    const std::vector<std::string>& arg_tokens) noexcept;

}  // namespace monitor::reading