#ifndef APP_APP_H
#define APP_APP_H

#include <config/filesystem_config.h>

namespace app {
class App final {
   public:
    explicit App(const std::filesystem::path& cfg_path);

    void run();

   private:
    config::FileSystemConfig cfg_;
};
}  // namespace app

#endif