#include <exception>
#include <iostream>

#include <config/config.hpp>
using config::Config;

int main(int argc, char* argv[]) {
    try {
        Config cfg = Config::prepare(argc, argv);
        std::cout << cfg.prompt << " " << cfg.logs << "\n";

    } catch (const std::exception& fatal_error) {
        std::cerr << fatal_error.what() << "\n";
        return 1;
    }
    return 0;
}