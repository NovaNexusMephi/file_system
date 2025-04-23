#pragma once

#include <filesystem>
#include <memory>
#include "spdlog/logger.h"
namespace fs = std::filesystem;

namespace monitor::logging {
/**
 * @brief A utility class responsible for logging command execution results.
 * 
 * This class provides an interface for recording the outcomes of commands (success or error) in a structured log file.
 * It uses the `spdlog` library to write logs to a file named `result.log` within a specified session directory.
 */
class ResultLogger final {
   public:
    /**
     * @brief Construct a new Result Logger object.
     * 
     * Initializes the `ResultLogger` by creating a logger instance that writes to a file named `result.log`
     * in the specified session directory. The log format includes a timestamp and the log message.
     * 
     * @param[in] session_path The path to the session directory where the log file will be stored.
     */
    explicit ResultLogger(const fs::path& session_path);

    /**
     * @brief Logs the successful execution of a command.
     * 
     * Records the successful outcome of a command, including its unique identifier and the result details.
     * 
     * @param[in] command_id A unique identifier for the command.
     * @param[in] result The result or output of the command execution.
     */
    void log_success(const std::string& command_id, const std::string& result);

    /**
     * @brief Logs the failure of a command.
     * 
     * Records the failure of a command, including its unique identifier and the associated error details.
     * 
     * @param[in] command_id A unique identifier for the command.
     * @param[in] error The error message describing the reason for the failure.
     */
    void log_error(const std::string& command_id, const std::string& error);

   private:
    std::shared_ptr<spdlog::logger> logger_;
};
}  // namespace monitor::logging