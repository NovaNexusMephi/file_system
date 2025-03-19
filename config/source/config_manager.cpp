#include "config/config_manager.h"

#include <filesystem>
#include <libconfig.h++>
#include <stdexcept>
#include <string>

#include "config/fat_config.h"
#include "config/system_info_config.h"
using config::ConfigManager;
using config::FATConfig;
using config::SystemInfoConfig;

ConfigManager::ConfigManager(const std::filesystem::path& cfg_path) {
    libconfig::Config cfg;
    try {
        cfg.readFile(cfg_path);
        read_fat_config(cfg);
        read_system_info_config(cfg);
    } catch (const libconfig::ParseException& parse_error) {
        std::ostringstream error_stream;
        error_stream << "invalid application config: parse error at " << parse_error.getFile() << ":"
                     << parse_error.getLine() << " - " << parse_error.getError();
        throw new std::runtime_error{error_stream.str()};
    } catch (const libconfig::FileIOException& io_error) {
        throw new std::runtime_error{"invalid application config: error reading config file"};
    } catch (const libconfig::SettingNotFoundException& not_found_error) {
        throw new std::runtime_error{"invalid application config: " + std::string(not_found_error.what())};
    } catch (const libconfig::SettingTypeException& type_error) {
        throw new std::runtime_error{"invalid application config: type mismatching for setting" +
                                     std::string(type_error.getPath())};
    }
}

const FATConfig& ConfigManager::get_fat_config() const noexcept { return fat_cfg_; }

const SystemInfoConfig& ConfigManager::get_system_info_config() const noexcept { return system_info_cfg_; }

void ConfigManager::read_fat_config(const libconfig::Config& cfg) {
    int segment_number = static_cast<int>(cfg.lookup("fat.segment_number"));
    if (!(MIN_SEGMENT_NUMBER <= segment_number && segment_number <= MAX_SEGMENT_NUMBER)) {
        throw std::out_of_range("invalid application config: segment number is out of available range");
    }
    fat_cfg_.segment_number = segment_number;
}

void ConfigManager::read_system_info_config(const libconfig::Config& cfg) {
    int major = static_cast<int>(cfg.lookup("system_info.version_major"));
    int minor = static_cast<int>(cfg.lookup("system_info.version_minor"));
    int patch = static_cast<int>(cfg.lookup("system_info.version_patch"));
    system_info_cfg_.version = SystemInfoConfig::Version{major, minor, patch};

    system_info_cfg_.volume_name = static_cast<std::string>(cfg.lookup("system_info.volume_name"));
    system_info_cfg_.owner_name = static_cast<std::string>(cfg.lookup("system_info.owner_name"));
    system_info_cfg_.system_name = static_cast<std::string>(cfg.lookup("system_info.system_name"));
}