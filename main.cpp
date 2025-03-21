#include <exception>
#include <filesystem>
#include <iostream>

#include "app/app_builder.h"

int main() {
    try {
        auto application = app::AppBuilder().set_cfg_path("configs/general.cfg").build();
        application->run();

    } catch (const std::exception& fatal_error) {
        std::cout << fatal_error.what() << "\n";
        return 1;
    }
    return 0;
}