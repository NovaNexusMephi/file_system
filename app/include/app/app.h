#ifndef _APP_APP_H_
#define _APP_APP_H_

#include "config/config.h"
#include <filesystem>
namespace app {

class App final {
public:
  explicit App(const std::filesystem::path &cfg_path = "");

  void run();

private:
  config::Config cfg_;
  bool working;
};
} // namespace app

#endif