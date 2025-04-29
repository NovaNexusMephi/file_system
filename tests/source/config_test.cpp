#include <gtest/gtest.h>

#include <algorithm>
#include <config/config.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <tuple>
#include <vector>
#include "gtest/gtest.h"
using config::Config;

using get_path_config_tc = std::tuple<std::vector<std::string>, std::string, bool>;
class GetCfgPathTest : public ::testing::TestWithParam<get_path_config_tc> {};

TEST_P(GetCfgPathTest, HandlesCommandLineArguments) {
    auto [args, expected_path, expect_exception] = GetParam();

    int argc = args.size();
    std::vector<char*> argv(argc);
    std::transform(args.begin(), args.end(), argv.begin(), [](std::string& s) { return &s[0]; });

    if (expect_exception) {
        EXPECT_THROW(Config::get_cfg_path(argc, argv.data()), std::runtime_error);
    } else {
        std::string result = Config::get_cfg_path(argc, argv.data());
        EXPECT_EQ(result, expected_path);
    }
}

const std::vector<get_path_config_tc> get_path_config_test_cases{
    std::make_tuple(std::vector<std::string>{"program", "-c", "config.json"}, "config.json", false),
    std::make_tuple(std::vector<std::string>{"program", "--config", "cfg.json"}, "cfg.json", false),
    std::make_tuple(std::vector<std::string>{"program", "-c", "a", "--config", "b"}, "b", false),
    std::make_tuple(std::vector<std::string>{"program"}, "", false),
    std::make_tuple(std::vector<std::string>{"program", "-c"}, "", true),
    std::make_tuple(std::vector<std::string>{"program", "--config"}, "", true),
    std::make_tuple(std::vector<std::string>{"program", "--unknown"}, "", true),
    std::make_tuple(std::vector<std::string>{"program", "-c", "a", "--invalid"}, "", true),
    std::make_tuple(std::vector<std::string>{"program", "positional", "-c", "cfg.json"}, "", true),
};

INSTANTIATE_TEST_SUITE_P(ConfigPathTests, GetCfgPathTest, ::testing::ValuesIn(get_path_config_test_cases));

using read_from_file_tc = std::tuple<std::string, Config, bool>;
class ReadFromFileTest : public ::testing::TestWithParam<read_from_file_tc> {
   protected:
    std::filesystem::path temp_file;

    void SetUp() override {
        auto [content, _, __] = GetParam();
        if (!content.empty()) {
            temp_file = std::filesystem::temp_directory_path() / "test_config.json";
            std::ofstream(temp_file) << content;
        }
    }

    void TearDown() override {
        if (!temp_file.empty() && std::filesystem::exists(temp_file)) {
            std::filesystem::remove(temp_file);
        }
    }
};

TEST_P(ReadFromFileTest, HandlesFileOperations) {
    auto [content, expected_config, expect_exception] = GetParam();

    if (expect_exception) {
        EXPECT_THROW(
            {
                try {
                    Config::read_from_file(temp_file);
                } catch (const nlohmann::json::exception& e) {
                    throw std::runtime_error(e.what());
                }
            },
            std::runtime_error);
    } else {
        Config cfg = Config::read_from_file(temp_file);
        EXPECT_EQ(cfg.prompt, expected_config.prompt);
        EXPECT_EQ(cfg.logs, expected_config.logs);
    }
}

const std::vector<read_from_file_tc> read_from_file_test_cases{
    std::make_tuple(
        R"({"prompt": ">>> ", "logs": "/var/log"})",
        []() {
            Config c;
            c.prompt = ">>> ";
            c.logs = "/var/log";
            return c;
        }(),
        false),
    std::make_tuple(R"({"prompt": "test"})", Config{}, true),
    std::make_tuple(R"({"prompt": 123, "logs": "/tmp"})", Config{}, true),
    std::make_tuple("{}", Config{}, true),
    std::make_tuple(R"({"logs": "/tmp"})", Config{}, true),
    std::make_tuple(R"({"prompt": "test", "logs": 456})", Config{}, true),
    std::make_tuple(
        R"({"prompt": "test", "logs": "/tmp", "extra": "value"})",
        []() {
            Config c;
            c.prompt = "test";
            c.logs = "/tmp";
            return c;
        }(),
        false),
    std::make_tuple(
        R"({"prompt": "test", "logs": "/tmp"})",
        []() {
            Config c;
            c.prompt = "test";
            c.logs = "/tmp";
            return c;
        }(),
        false),
    std::make_tuple(
        R"({"prompt": "", "logs": ""})",
        []() {
            Config c;
            c.prompt = "";
            c.logs = "";
            return c;
        }(),
        false),
    std::make_tuple(
        R"({"prompt": "test", "logs": "../logs"})",
        []() {
            Config c;
            c.prompt = "test";
            c.logs = "../logs";
            return c;
        }(),
        false),
    std::make_tuple(
        R"({"prompt": "test", "logs": "C:\\Program Files\\"})",
        []() {
            Config c;
            c.prompt = "test";
            c.logs = "C:\\Program Files\\";
            return c;
        }(),
        false),
    std::make_tuple(
        R"({"prompt": "  test  ", "logs": "/tmp"})",
        []() {
            Config c;
            c.prompt = "  test  ";
            c.logs = "/tmp";
            return c;
        }(),
        false),
    std::make_tuple(
        R"({"prompt": "test", "logs": "/папка с пробелами"})",
        []() {
            Config c;
            c.prompt = "test";
            c.logs = "/папка с пробелами";
            return c;
        }(),
        false),
    std::make_tuple(
        R"({"prompt": "line1\nline2", "logs": "/tmp"})",
        []() {
            Config c;
            c.prompt = "line1\nline2";
            c.logs = "/tmp";
            return c;
        }(),
        false),
    std::make_tuple("[1, 2, 3]", Config{}, true),
    std::make_tuple("invalid json", Config{}, true),
    std::make_tuple(R"({"prompt": null, "logs": "/tmp"})", Config{}, true),
    std::make_tuple(
        R"({"prompt": ")" + std::string(10000, 'a') + R"(", "logs": "/tmp"})",
        []() {
            Config c;
            c.prompt = std::string(10000, 'a');
            c.logs = "/tmp";
            return c;
        }(),
        false),
};

INSTANTIATE_TEST_SUITE_P(ReadFromFileTests, ReadFromFileTest, ::testing::ValuesIn(read_from_file_test_cases));

TEST(ReadFromFileDeathTest, HandlesMissingFile) {
    std::filesystem::path invalid_path = "/invalid/path/config.json";
    EXPECT_THROW(Config::read_from_file(invalid_path), std::runtime_error);
}

TEST(ReadFromFileDeathTest, HandlesDirectoryPath) {
    std::filesystem::path dir_path = std::filesystem::temp_directory_path();
    EXPECT_THROW(Config::read_from_file(dir_path), std::runtime_error);
}