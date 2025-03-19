#include <exception>
#include <filesystem>
#include <iostream>

#include "config/config_manager.h"

int main() {
    try {
        std::filesystem::path cfg_path{"configs/general.cfg"};
        config::ConfigManager cfg_manager_{cfg_path};

        std::cout << cfg_manager_.get_fat_config().segment_number << "\n";
        std::cout << cfg_manager_.get_system_info_config().volume_name << "\n";
        std::cout << cfg_manager_.get_system_info_config().owner_name << "\n";
        std::cout << cfg_manager_.get_system_info_config().system_name << "\n";
        std::cout << cfg_manager_.get_system_info_config().version.major << "\n";
        std::cout << cfg_manager_.get_system_info_config().version.minor << "\n";
        std::cout << cfg_manager_.get_system_info_config().version.patch << "\n";
    } catch (const std::exception& fatal_error) {
        std::cout << fatal_error.what() << "\n";
        return 1;
    }
    return 0;
}