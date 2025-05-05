#pragma once

#include <filesystem>
namespace config {

/**
 * @brief Configuration structure for application settings
 * 
 * Manages application configuration parameters with default values.
 * Provides methods for loading configurations from JSON files and 
 * command-line arguments parsing.
 */
struct Config final {
   public:
    /**
     * @brief Default constructor
     * 
     * Initializes configuration with default values:
     * - prompt: "# "
     * - logs: "./logs"
     */
    Config() = default;

    /**
     * @brief Prepare configuration from command-line arguments
     * 
     * Parses command-line arguments to find configuration path.
     * If path is provided via -c/--config, loads configuration from file.
     * Otherwise returns default configuration.
     * 
     * @param[in] argc Argument count from main()
     * @param[in] argv Argument values from main()
     * @return Config Loaded configuration (either from file or defaults)
     * 
     * @throws std::runtime_error if:
     *   - Invalid command-line arguments
     *   - Configuration file loading fails
     */
    [[nodiscard]] static Config prepare(int argc, char* argv[]);

    /**
     * @brief Load configuration from JSON file
     * 
     * @param[in] cfg_path Path to configuration file (must be a valid JSON file)
     * @return Config Loaded configuration object
     * 
     * @throws std::runtime_error if:
     *   - File doesn't exist
     *   - Path points to non-regular file
     *   - File cannot be opened
     *   - JSON parsing fails
     *   - Required fields are missing in JSON
     */
    [[nodiscard]] static Config read_from_file(const std::filesystem::path& cfg_path);

    /**
     * @brief Extract configuration path from command-line arguments
     * 
     * Looks for -c/--config flags in arguments. Expects format:
     *   -c /path/to/config.json
     *   --config /path/to/config.json
     * 
     * @param[in] argc Argument count from main()
     * @param[in] argv Argument values from main()
     * @return std::string Configuration file path, or empty string if not found
     * 
     * @throws std::runtime_error if:
     *   - Arguments contain unknown flags
     *   - -c/--config flag has no value
     */
    [[nodiscard]] static std::string get_cfg_path(int argc, char* argv[]);

   public:
    /// Default command prompt format
    static constexpr std::string DEFAULT_PROMPT{"# "};
    /// Default directory for log files
    static constexpr std::string DEFAULT_LOGS{"./logs"};

    /// Command prompt format (default: "# ")
    std::string prompt{DEFAULT_PROMPT};
    /// Path to log directory (default: "./logs")
    std::filesystem::path logs{DEFAULT_LOGS};
};
}  // namespace config