#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>

int main() {
    try {
        // TODO: Initialize config from file

        // TODO: Initialize logger

        // TODO: Initialize model of filesystem

        // TODO: Initialize monitor

    } catch (const std::exception& fatal_error) {
        std::cout << fatal_error.what() << std::endl;
        return 1;
    }
    return 0;
}