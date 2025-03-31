#include <exception>
#include <iostream>

int main() {
    try {
        std::cout << "Hello, world!" << std::endl;
    } catch (const std::exception& fatal_error) {
        std::cout << fatal_error.what() << std::endl;
        return 1;
    }
    return 0;
}