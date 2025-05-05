#pragma once

#include <filesystem>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
namespace monitor {
/**
 * @brief Monitoring system for command processing and logging
 * 
 * Manages user input processing, command parsing, and session logging.
 * Creates timestamped session directories with result and history logs.
 */
class Monitor final {
   public:
    /**
      * @brief Constructor
      * 
      * Initializes monitor with specified prompt and log directory.
      * Creates new session directory and log files.
      * 
      * @param[in] prompt Command prompt string
      * @param[in] logs Base directory for session logs
      */
    Monitor(const std::string& prompt, const std::string& logs);

    /**
      * @brief Destructor
      * 
      * Ensures proper closure of log files
      */
    ~Monitor();

    /**
      * @brief Get and parse user command
      * 
      * Reads input until ';' delimiter, parses using Parser::parse()
      * 
      * @return Parsed JSON command if successful, nullopt on EOF or parse error
      */
    [[nodiscard]] std::optional<nlohmann::json> get_command() noexcept;

    /**
      * @brief Write output message
      * 
      * Writes message to stdout and appends to results.log
      * 
      * @param[in] msg Message to write
      */
    void write(const std::string& msg);

   private:
    /**
      * @brief Generate ISO 8601 timestamp with milliseconds
      * 
      * Format: YYYY-MM-DD_HH-MM-SS.sss
      * 
      * @return Timestamp string
      */
    [[nodiscard]] std::string generate_timestamp() noexcept;

    /**
      * @brief Create session directory
      * 
      * Creates directory structure: [logs]/YYYY-MM-DD_HH-MM-SS.sss/
      * 
      * @param[in] d_path Base log directory
      * @return Full path to created session directory
      */
    std::filesystem::path create_session_dir(const std::filesystem::path& d_path);

    /**
      * @brief Create session log files
      * 
      * Creates:
      * - results.log for command outputs
      * - history.log for command history
      * 
      * @param[in] s_dir Session directory path
      */
    void create_log_files(const std::filesystem::path& s_dir);

    std::string prompt_;          ///< Command prompt string
    std::filesystem::path logs_;  ///< Base log directory path
    std::string last_command_;    ///< Last received raw command string

    std::ofstream results_;  ///< Results log file stream
    std::ofstream history_;  ///< Command history log stream
};
}  // namespace monitor