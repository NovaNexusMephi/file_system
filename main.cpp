#include <app/app.h>
#include <exception>
#include <iostream>

// TODO: Make parsing of command line
int main() {
  try {
    app::App application;
    application.run();
  } catch (const std::exception &fatal_error) {
    std::cerr << fatal_error.what() << "\n";
    return 1;
  }
  return 0;
}