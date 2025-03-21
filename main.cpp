#include <exception>
#include <iostream>
#include "app/include/app/app.h"

int main() {
    try {
        app::App application{"configs/filesystem.cfg"};
        application.run();
    } catch (const std::exception& fatal_error) {
        std::cout << fatal_error.what() << "\n";
        return 1;
    }
    return 0;
}