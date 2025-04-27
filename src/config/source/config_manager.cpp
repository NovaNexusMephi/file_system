#include "config/config_manager.hpp"
#include <filesystem>
#include "config/filesystem_config.hpp"
#include "config/monitor_config.hpp"
using config::ConfigManager;

ConfigManager::ConfigManager(const std::filesystem::path& cfg_path) {
    if (!std::filesystem::exists(cfg_path)) {
        throw std::runtime_error("Configuration file not found: " + cfg_path.string());
    }

    try {
        libconfig::Config config;
        config.readFile(cfg_path.string().c_str());

        const libconfig::Setting& root = config.getRoot();
        fs_config_ = FilesystemConfig(root["filesystem"]);
        monitor_config_ = MonitorConfig(root["monitor"]);

    } catch (const libconfig::FileIOException& e) {
        throw std::runtime_error("Error reading configuration file: " + std::string(e.what()));
    } catch (const libconfig::ParseException& e) {
        throw std::runtime_error("Parse error at " + std::string(e.getFile()) + ":" + std::to_string(e.getLine()) +
                                 " - " + std::string(e.getError()));
    } catch (const libconfig::SettingNotFoundException& e) {
        throw std::runtime_error("Missing required configuration section: " + std::string(e.getPath()));
    }
}

const config::FilesystemConfig& ConfigManager::get_fs_config() const noexcept {
    return fs_config_;
}

const config::MonitorConfig& ConfigManager::get_monitor_config() const noexcept {
    return monitor_config_;
}