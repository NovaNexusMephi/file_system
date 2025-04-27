#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <config/monitor_config.hpp>
#include <libconfig.h++>
#include <vector>
#include "gtest/gtest.h"
using config::InputConfig;
using config::MonitorConfig;
using config::OutputConfig;

struct InputConfigTestCase {
    std::string prompt;
    bool wait_for_user_input;
    std::string from;
};

class InputConfigTest : public ::testing::TestWithParam<InputConfigTestCase> {
   protected:
    libconfig::Config cfg;
};

TEST_P(InputConfigTest, ParsesInputConfigCorrectly) {
    const auto& [expected_prompt, expected_wait_for_user_input, expected_from] = GetParam();

    cfg.getRoot().add("input", libconfig::Setting::TypeGroup);
    auto& input_setting = cfg.lookup("input");
    input_setting.add("prompt", libconfig::Setting::TypeString) = expected_prompt;
    input_setting.add("wait_for_input", libconfig::Setting::TypeBoolean) = expected_wait_for_user_input;
    input_setting.add("from", libconfig::Setting::TypeString) = expected_from;

    InputConfig input_config(input_setting);

    EXPECT_EQ(input_config.prompt, expected_prompt);
    EXPECT_EQ(input_config.wait_for_input, expected_wait_for_user_input);
    EXPECT_EQ(input_config.from, expected_from);
}

const std::vector<InputConfigTestCase> input_config_test_cases{
    {"[prompt]", true, "file.log"},
    {"[prompt]", true, ""},
    {"[prompt]", false, "file.log"},
    {"[prompt]", false, ""},
    {"[default prompt]", true, "file.log"},
    {"[default prompt]", true, ""},
    {"[default prompt]", false, "file.log"},
    {"[default prompt]", false, ""},
    {"", true, "file.log"},
    {"", true, ""},
    {"", false, "file.log"},
    {"", false, ""},
};

INSTANTIATE_TEST_SUITE_P(InputConfigTests, InputConfigTest, testing::ValuesIn(input_config_test_cases));

struct OutputConfigTestCase {
    std::string expected_to;
};

class OutputConfigTest : public ::testing::TestWithParam<OutputConfigTestCase> {
   protected:
    libconfig::Config cfg;
};

TEST_P(OutputConfigTest, ParsesOutputConfigCorrectly) {
    const auto& [expected_to] = GetParam();

    cfg.getRoot().add("output", libconfig::Setting::TypeGroup);
    auto& output_setting = cfg.lookup("output");
    output_setting.add("to", libconfig::Setting::TypeString) = expected_to;

    OutputConfig output_config(output_setting);

    EXPECT_EQ(output_config.to, expected_to);
}

const std::vector<OutputConfigTestCase> output_config_test_cases{
    {"file.log"},
    {"stdout"},
    {"stderr"},
    {"/dev/null"},
    {"C:\\path\\file"},
    {""},
    {" "},
    {"../relative/path.log"},
    {"~/user_file.log"},
    {"\\\\server\\share\\file.log"},
    {"файл_вывода.log"},
    {"output_with_spaces .log"},
    {"very/long/path/to/output/file.log"},
    {"special!@#$%^&*().log"},
    {"fifo:///tmp/my_fifo"},
};

INSTANTIATE_TEST_SUITE_P(OutputConfigTests, OutputConfigTest, testing::ValuesIn(output_config_test_cases));

struct MonitorConfigTestCase {
    std::string input_prompt;
    bool input_wait_for_user_input;
    std::string input_from;
    std::string output_to;
};

class MonitorConfigTest : public ::testing::TestWithParam<MonitorConfigTestCase> {
   protected:
    libconfig::Config cfg;
};

TEST_P(MonitorConfigTest, ParsesMonitorConfigCorrectly) {
    const auto& [input_prompt, input_wait_for_user_input, input_from, output_to] = GetParam();
    cfg.getRoot().add("monitor", libconfig::Setting::TypeGroup);
    auto& monitor_setting = cfg.lookup("monitor");

    monitor_setting.add("input", libconfig::Setting::TypeGroup);
    auto& input_setting = monitor_setting.lookup("input");
    input_setting.add("prompt", libconfig::Setting::TypeString) = input_prompt;
    input_setting.add("wait_for_input", libconfig::Setting::TypeBoolean) = input_wait_for_user_input;
    input_setting.add("from", libconfig::Setting::TypeString) = input_from;

    monitor_setting.add("output", libconfig::Setting::TypeGroup);
    auto& output_setting = monitor_setting.lookup("output");
    output_setting.add("to", libconfig::Setting::TypeString) = output_to;

    MonitorConfig monitor_config(monitor_setting);

    EXPECT_EQ(monitor_config.input.prompt, input_prompt);
    EXPECT_EQ(monitor_config.input.wait_for_input, input_wait_for_user_input);
    EXPECT_EQ(monitor_config.input.from, input_from);

    EXPECT_EQ(monitor_config.output.to, output_to);
}

const std::vector<MonitorConfigTestCase> monitor_config_test_cases{
    {"[prompt]", true, "file.log", "stdout"},
    {"[prompt]", false, "", "stderr"},
    {"[default prompt]", true, "data.txt", "/dev/null"},
    {"", false, "", "file.log"},
    {"[prompt]", true, "file.log", ""},
    {"[empty input]", true, "", "stdout"},
    {"[no prompt]", false, "input.txt", "stderr"},
    {"[custom prompt]", true, "data.log", "/dev/null"},
    {"[long prompt]", true, "large_file.log", "output.txt"},
    {"[minimal]", false, "", ""},
    {"[windows path]", true, "C:\\input.txt", "D:\\output.txt"},
    {"[special device]", false, "/dev/random", "/dev/null"},
    {"[network path]", true, "\\\\server\\share\\input.txt", "\\\\server\\share\\output.txt"},
    {"[unicode prompt]", true, "файл_ввода.log", "файл_вывода.log"},
    {"[edge case]", false, " ", " "}};

INSTANTIATE_TEST_SUITE_P(MonitorConfigTests, MonitorConfigTest, testing::ValuesIn(monitor_config_test_cases));