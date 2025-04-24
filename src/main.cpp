#include <exception>
#include <iostream>

#include <nlohmann/json.hpp>
#include "monitor/scanners/console_scanner.hpp"

int main() {
    try {
        monitor::scanners::ConsoleScanner scanner{};
        while (scanner.has_next()) {
            std::cout << scanner.next() << "\n";
        }
    } catch (const std::exception& fatal_error) {
        std::cout << fatal_error.what() << std::endl;
        return 1;
    }
    return 0;
}