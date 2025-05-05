#include "config/config.hpp"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
using config::Config;
using json = nlohmann::json;

Config Config::prepare(int argc, char* argv[]) {
    auto cfg_path = get_cfg_path(argc, argv);
    if (cfg_path.empty()) {
        return Config{};
    }
    return read_from_file(cfg_path);
}

Config Config::read_from_file(const std::filesystem::path& cfg_path) {
    if (!std::filesystem::exists(cfg_path)) {
        throw std::runtime_error{"configuration file was not found"};
    }
    if (!std::filesystem::is_regular_file(cfg_path)) {
        throw std::runtime_error{"configuration file must be a regular file"};
    }

    std::ifstream stream{cfg_path};
    if (!stream.is_open()) {
        throw std::runtime_error{"cannot open configuration file"};
    }
    json cfg_data = json::parse(stream);

    Config cfg_inst;
    cfg_inst.prompt = cfg_data["prompt"].get<std::string>();
    cfg_inst.logs = cfg_data["logs"].get<std::string>();

    return cfg_inst;
}

std::string Config::get_cfg_path(int argc, char** argv) {
    std::string config_path{};
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if ((arg == "-c" || arg == "--config") && i + 1 < argc) {
            config_path = argv[++i];
        } else {
            throw std::runtime_error{"unknown argument"};
        }
    }
    return config_path;
}
