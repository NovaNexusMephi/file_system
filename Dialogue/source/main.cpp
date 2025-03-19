#include <exception>
#include <iostream>

#include <config/config.h>

int main() {
  config::Config cfg;
  auto fat_cfg = cfg.get_fat_config();
  return 0;
}
