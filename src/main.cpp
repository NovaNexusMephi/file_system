#include "monitor/reader.h"
#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>

int main() {
  try {
    // TODO: Initialize config from file

    // TODO: Initialize logger

    // TODO: Initialize model of filesystem

    // TODO: Initialize monitor
    monitor::Reader reader{"> "};

    while (true) {
      auto precommand = reader.read(std::cin);
      std::cout << (precommand.has_value() ? *precommand : "nullopt") << "\n";
    }

  } catch (const std::exception &fatal_error) {
    std::cout << fatal_error.what() << std::endl;
    return 1;
  }
  return 0;
}