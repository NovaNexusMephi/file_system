#include <exception>
#include <iostream>
#include "config/filesystem_config.h"

int main() {
    try {
        config::FileSystemConfig cfg{"configs/filesystem.cfg"};
    } catch (const std::exception& fatal_error) {
        std::cout << fatal_error.what() << "\n";
        return 1;
    }
    return 0;
}