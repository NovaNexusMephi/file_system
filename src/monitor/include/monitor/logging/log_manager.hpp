#pragma once

#include <filesystem>
#include <memory>
#include "monitor/logging/history_logger.hpp"
#include "monitor/logging/result_logger.hpp"
namespace fs = std::filesystem;

namespace monitor::logging {

/**
 * @brief A utility class responsible for managing logging sessions and loggers.
 * 
 * This class encapsulates the creation of session directories and provides access to loggers for recording history and results.
 * It ensures that all logs are stored in a structured manner under a unique session directory.
 */
class LogManager final {
   public:
    /**
     * @brief Construct a new Log Manager object.
     * 
     * Initializes the LogManager by creating a unique session directory and initializing the `HistoryLogger` and `ResultLogger` instances.
     */
    LogManager();

    /**
     * @brief Get the session path object.
     * 
     * Returns the file system path of the current session directory where logs are stored.
     * 
     * @return fs::path The path to the session directory.
     */
    [[nodiscard]] fs::path get_session_path() const noexcept;

    /**
     * @brief Get the history logger object.
     * 
     * Provides access to the `HistoryLogger` instance, which is responsible for recording command history.
     * 
     * @return const std::shared_ptr<HistoryLogger>& A shared pointer to the `HistoryLogger` instance.
     */
    [[nodiscard]] const std::shared_ptr<HistoryLogger>& get_history_logger() const;

    /**
     * @brief Get the result logger object.
     * 
     * Provides access to the `ResultLogger` instance, which is responsible for recording command execution results.
     * 
     * @return const std::shared_ptr<ResultLogger>& A shared pointer to the `ResultLogger` instance.
     */
    [[nodiscard]] const std::shared_ptr<ResultLogger>& get_result_logger() const;

   private:
    void create_session_directory();

    fs::path session_path_{};
    std::shared_ptr<HistoryLogger> history_logger_{nullptr};
    std::shared_ptr<ResultLogger> result_logger_{nullptr};
};
}  // namespace monitor::logging