#include <cstdlib>
#include <exception>
#include <iostream>

int main() {
    try {
    } catch (const std::exception& fatal_error) {
        std::cerr << fatal_error.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}