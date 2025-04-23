#pragma once

#include <memory>
#include "spdlog/logger.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace monitor::logging {
/**
 * @brief A utility class responsible for logging command history.
 * 
 * This class provides an interface for recording the lifecycle of commands (start, commit, rollback) in a structured log file.
 * It uses the `spdlog` library to write logs to a file named `history.log` within a specified session directory.
 */
class HistoryLogger final {
   public:
    /**
     * @brief Construct a new History Logger object.
     * 
     * Initializes the `HistoryLogger` by creating a logger instance that writes to a file named `history.log`
     * in the specified session directory. The log format includes a timestamp and the log message.
     * 
     * @param[in] session_path The path to the session directory where the log file will be stored.
     */
    explicit HistoryLogger(const fs::path& session_path);

    /**
     * @brief Logs the start of a command execution.
     * 
     * Records the initiation of a command with its unique identifier and the command string.
     * 
     * @param[in] command_id A unique identifier for the command.
     * @param[in] command The command string being executed.
     */
    void log_start(const std::string& command_id, const std::string& command);

    /**
     * @brief Logs the successful completion (commit) of a command.
     * 
     * Records the successful execution of a command using its unique identifier.
     * 
     * @param[in] command_id A unique identifier for the command.
     */
    void log_commit(const std::string& command_id);

    /**
     * @brief Logs the failure (rollback) of a command.
     * 
     * Records the failure of a command along with the associated error message.
     * 
     * @param[in] command_id A unique identifier for the command.
     * @param[in] error The error message describing the reason for the rollback.
     */
    void log_rollback(const std::string& command_id, const std::string& error);

   private:
    std::shared_ptr<spdlog::logger> logger_;
};
}  // namespace monitor::logging