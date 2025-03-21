#ifndef APP_APP_BUILDER_H
#define APP_APP_BUILDER_H

#include <filesystem>
#include <memory>

#include "app/app.h"

namespace app {

/// @brief Builder class for configuring and constructing `App` instances.
/// Provides a fluent interface to set parameters incrementally.
class AppBuilder final {
 public:
  /**
     * @brief Sets the configuration file path for the application.
     * @param[in] cfg_path The filesystem path to the configuration file.
     * @return Reference to this builder for method chaining [[5]].
     */
  AppBuilder& set_cfg_path(const std::filesystem::path& cfg_path);

  /*!
     * @brief Constructs the `App` instance with the configured parameters.
     * @return A `unique_ptr` to the fully initialized `App` object.
     */
  [[nodiscard]] std::unique_ptr<App> build();

 private:
  std::filesystem::path
      cfg_path_;  ///< Path to the application's configuration file.
};
}  // namespace app

#endif