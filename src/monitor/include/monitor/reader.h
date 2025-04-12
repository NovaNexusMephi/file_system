#pragma once

#include <istream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace monitor {
/**
 * @brief A utility class for parsing commands from an input stream into a structured JSON format.
 * 
 * The `Reader` class provides methods to read and parse commands from an input stream. It converts raw command strings
 * into a structured JSON representation, making it easier to process and interpret commands programmatically.
 * 
 * The expected format of a command string is:
 * `<command_name> <data> /option1:value1 /option2:value2 ...`
 * 
 * Example Usage:
 * ```
 * std::istringstream input("create file.txt /allocate:10");
 * auto result = Reader::read(input);
 * if (result.has_value()) {
 *     std::cout << result->dump(4) << std::endl;
 * }
 * ```
 * Output:
 * ```json
 * {
 *     "name": "create",
 *     "data": ["file.txt"],
 *     "options": {
 *         "allocate": 10
 *     }
 * }
 */
class Reader final {
   public:
    Reader() = default;

    /**
     * @brief Reads and parses a command from the input stream.
     * 
     * This function reads a command string from the provided input stream, parses it into a structured JSON format,
     * and returns the result. If the input is invalid or cannot be parsed, the function returns an empty optional.
     * 
     * The expected format of the command in the input stream is:
     * `<command_name> <data> /option1:value1 /option2:value2 ...`
     * 
     * @param[in] from The input stream to read the command from. The stream should contain a single command string.
     * @return std::optional<nlohmann::json> 
     *         A JSON object representing the parsed command if successful. The JSON object will have the following structure:
     *         {
     *             "name": "<command_name>",
     *             "data": ["<data1>", "<data2>", ...],
     *             "options": {
     *                 "<option1>": <value1>,
     *                 "<option2>": <value2>,
     *                 ...
     *             }
     *         }
     *         If parsing fails, the function returns an empty optional (`std::nullopt`).
     */
    [[nodiscard]] static std::optional<nlohmann::json> read(std::istream& from) noexcept;

   private:
    [[nodiscard]] static std::optional<nlohmann::json> parse_command(const std::string& source) noexcept;

    [[nodiscard]] static std::vector<std::string> split(const std::string& src, const std::string& delims) noexcept;

    [[nodiscard]] static nlohmann::json make_options(const std::vector<std::string>& opt_src) noexcept;

    [[nodiscard]] static std::pair<std::string, std::string> split_opt(const std::string& opt_token);
};
}  // namespace monitor