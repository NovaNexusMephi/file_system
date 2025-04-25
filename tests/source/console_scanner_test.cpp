#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

#include "monitor/io/console/console_scanner.hpp"
using monitor::io::console::ConsoleScanner;

struct ConsoleScannerTestParams {
    std::string input;
    std::vector<std::string> expected;
};

class ConsoleScannerTestFixture : public ::testing::TestWithParam<ConsoleScannerTestParams> {
   protected:
    void SetUp() override {
        orig_cin_buf = std::cin.rdbuf(fake_input.rdbuf());
        fake_input.str(GetParam().input);
    }

    void TearDown() override { std::cin.rdbuf(orig_cin_buf); }

    std::stringstream fake_input;
    std::streambuf* orig_cin_buf;
    ConsoleScanner scanner;
};

TEST_P(ConsoleScannerTestFixture, NextReturnsExpectedCommands) {
    const auto& expected_commands = GetParam().expected;
    for (const auto& expected_cmd : expected_commands) {
        ASSERT_TRUE(scanner.has_next()) << "Failed at command: " << expected_cmd;
        EXPECT_EQ(scanner.next(), expected_cmd);
    }
    EXPECT_FALSE(scanner.has_next()) << "Expected no more commands";
}

std::vector<ConsoleScannerTestParams> console_scanner_test_cases = {
    {"cmd1;cmd2;cmd3", {"cmd1", "cmd2", "cmd3"}},
    {"  cmd1  ;  cmd2  ;  cmd3  ", {"cmd1", "cmd2", "cmd3"}},
    {";;;cmd1;;cmd2;", {"cmd1", "cmd2"}},
    {"single_command", {"single_command"}},
    {"\t\tcmd1\t\t;\t\tcmd2\t\t", {"cmd1", "cmd2"}},
    {"cmd1\n;\ncmd2\n;\ncmd3", {"cmd1", "cmd2", "cmd3"}},
    {"   \t   cmd1   \t   ;   \t   cmd2   \t   ", {"cmd1", "cmd2"}},
    {"", {}},
    {"cmd1; ; ;cmd2; ;cmd3", {"cmd1", "cmd2", "cmd3"}},
    {"привет;мир;!", {"привет", "мир", "!"}},
    {"\"quoted\";'single';@special", {"\"quoted\"", "'single'", "@special"}},
    {"cmd1;\n\ncmd2;\n\ncmd3", {"cmd1", "cmd2", "cmd3"}},
    {"cmd1;", {"cmd1"}},
    {"cmd1;cmd2;cmd3   ", {"cmd1", "cmd2", "cmd3"}},
    {"   cmd1;cmd2;cmd3", {"cmd1", "cmd2", "cmd3"}},
    {"   \t   cmd1   \t   ;\n\ncmd2\n;\tcmd3\t", {"cmd1", "cmd2", "cmd3"}},
    {"cmd1;cmd2;cmd3", {"cmd1", "cmd2", "cmd3"}},
    {"this_is_a_very_long_command_without_semicolon", {"this_is_a_very_long_command_without_semicolon"}},
    {"cmd1 ;cmd2; cmd3 ; cmd4", {"cmd1", "cmd2", "cmd3", "cmd4"}},
};

INSTANTIATE_TEST_SUITE_P(ConsoleScannerTests, ConsoleScannerTestFixture,
                         ::testing::ValuesIn(console_scanner_test_cases));