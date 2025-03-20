#ifndef APP_APP_H
#define APP_APP_H

#include <config/config_manager.h>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

#include <atomic>
#include <filesystem>
#include <memory>

namespace app {

class AppBuilder;

/**
 * @brief Main application class responsible for initialization, execution, and cleanup.
 * Encapsulates the application lifecycle and signal handling [[9]].
 */
class App final {
    friend class AppBuilder;

   public:
    /**
     * @brief Constructs the application with a configuration file path.
     * @param cfg_path Path to the configuration file.
     */
    explicit App(const std::filesystem::path& cfg_path);

    /**
     * @brief Starts the application's main loop and handles termination signals.
     * Registers signal handlers for graceful shutdown and runs the main loop [[7]].
     */
    void run();

   private:
    /// Performs initialization tasks before entering the main loop.
    void start_up();

    /// Cleans up resources after the main loop exits.
    void shutdown();

    /**
     * @brief Handles `SIGINT` and `SIGTERM` to trigger a clean shutdown.
     * @param[in] signal received signal number (e.g., `SIGINT`).
     */
    static void signal_handler(int signal);

    config::ConfigManager cfg_manager_;       ///< Manages configuration settings
    std::atomic<bool> working_{true};         ///< Controls the main loop execution
    std::shared_ptr<spdlog::logger> logger_;  ///< Logs system information
};
}  // namespace app

#endif