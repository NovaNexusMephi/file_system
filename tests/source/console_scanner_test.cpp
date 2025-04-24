#include <gtest/gtest.h>
#include <sstream>
#include <streambuf>

#include "monitor/scanners/console_scanner.hpp"

using monitor::scanners::ConsoleScanner;

struct ConsoleScannerTestParams {
    std::string description;
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

const std::vector<ConsoleScannerTestParams> console_scanner_tests_cases{
    {"Single command", "cmd;", {"cmd"}},
    {"Multiple commands with spaces", "  cmd1 ; cmd2  ;  cmd3 ;", {"cmd1", "cmd2", "cmd3"}},
    {"Previously comments now commands", "#cmd1; #cmd2;", {"#cmd1", "#cmd2"}},
    {"Mixed content", "  ; #comment ; cmd ; ; #comment2", {"#comment", "cmd", "#comment2"}},
    {"No commands", "", {}},
    {"Command without semicolon", "cmd", {"cmd"}},
    {"Previously comments now valid",
     "# comment1 ;  cmd  ; # comment2 ; cmd2 ;",
     {"# comment1", "cmd", "# comment2", "cmd2"}},
};

INSTANTIATE_TEST_SUITE_P(ConsoleScannerTests, ConsoleScannerTestFixture,
                         ::testing::ValuesIn(console_scanner_tests_cases));