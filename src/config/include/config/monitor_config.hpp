#pragma once

#include <libconfig.h++>
#include <string>
namespace config {

/**
 * @brief Represents the configuration for input settings.
 * 
 * This structure encapsulates the parameters related to input handling, such as the prompt message, 
 * whether to wait for user input, and the source of the input. Default values are provided for all fields.
 */
struct InputConfig final {
    /// The message displayed to the user when prompting for input.
    std::string prompt{};
    /// Indicates whether the program should wait for user input.
    bool wait_for_input{};
    /// The source of the input (e.g., file, stream, etc.).
    std::string from{};

    InputConfig() = default;

    /**
     * @brief Construct a new Input Config object from a libconfig::Setting.
     * 
     * Initializes the InputConfig object by reading values from the provided `settings`. 
     * If specific keys are missing or invalid, default values are used where applicable.
     * 
     * @param[in] settings The libconfig::Setting object containing the input configuration.
     */
    InputConfig(const libconfig::Setting& settings);
};

/**
 * @brief Represents the configuration for output settings.
 * 
 * This structure encapsulates the parameters related to output handling, such as the destination of the output. 
 * All fields must be explicitly defined in the configuration, as no default values are provided.
 */
struct OutputConfig final {
    /// The destination of the output (e.g., file, stream, etc.).
    std::string to{};

    OutputConfig() = default;

    /**
     * @brief Construct a new Output Config object from a libconfig::Setting.
     * 
     * Initializes the OutputConfig object by reading values from the provided `settings`. 
     * Throws an exception if the required `to` key is missing or invalid.
     * 
     * @param[in] settings The libconfig::Setting object containing the output configuration.
     */
    OutputConfig(const libconfig::Setting& settings);
};

/**
 * @brief Represents the configuration for monitoring settings.
 * 
 * This structure encapsulates the overall configuration for a monitoring system, including 
 * both input and output configurations. It aggregates instances of `InputConfig` and `OutputConfig`.
 */
struct MonitorConfig final {
    /// The input configuration for the monitoring system.
    InputConfig input;
    /// The output configuration for the monitoring system.
    OutputConfig output;

    MonitorConfig() = default;

    /**
     * @brief Construct a new Monitor Config object from a libconfig::Setting.
     * 
     * Initializes the MonitorConfig object by reading values from the provided `settings`. 
     * The `input` and `output` configurations are initialized using their respective constructors.
     * 
     * @param[in] settings The libconfig::Setting object containing the monitoring configuration.
     */
    MonitorConfig(const libconfig::Setting& settings);
};
}  // namespace config