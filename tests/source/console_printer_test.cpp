#include "monitor/io/console/console_printer.hpp"
#include <gtest/gtest.h>
#include <sstream>
using monitor::io::console::ConsolePrinter;

struct ConsolePrinterTestParams {
    std::string input;
    std::string expected;
};

class ConsolePrinterTestFixture : public ::testing::TestWithParam<ConsolePrinterTestParams> {
   protected:
    void SetUp() override { orig_cout_buf = std::cout.rdbuf(fake_output.rdbuf()); }

    void TearDown() override { std::cout.rdbuf(orig_cout_buf); }

    std::stringstream fake_output;
    std::streambuf* orig_cout_buf;
    ConsolePrinter printer;
};

TEST_P(ConsolePrinterTestFixture, PrintWritesExpectedOutput) {
    const auto& test_params = GetParam();
    printer.print(test_params.input);
    EXPECT_EQ(fake_output.str(), test_params.expected) << "Failed for input: " << test_params.input;
}

std::vector<ConsolePrinterTestParams> console_printer_test_cases = {
    {"Hello, World!", "Hello, World!"},
    {"", ""},
    {"\tTabbed Text\n", "\tTabbed Text\n"},
    {"Привет, мир!", "Привет, мир!"},
    {"12345", "12345"},
    {"Special chars: @#$%^&*", "Special chars: @#$%^&*"},
    {"Multiple lines\nSecond line", "Multiple lines\nSecond line"},
    {"Single quote: '", "Single quote: '"},
    {"Double quotes: \"\"", "Double quotes: \"\""},
    {"Backslash escape: \\", "Backslash escape: \\"},
    {"Mixed quotes: '\"\\", "Mixed quotes: '\"\\"},
    {"Unicode symbols: ❤️🌍🚀", "Unicode symbols: ❤️🌍🚀"},
    {"Long string: " + std::string(1000, 'a'), "Long string: " + std::string(1000, 'a')},
    {"Trailing spaces   ", "Trailing spaces   "},
    {"Leading spaces:    text", "Leading spaces:    text"},
    {"Tabs and spaces: \t \t text", "Tabs and spaces: \t \t text"},
    {"Empty lines:\n\n\n", "Empty lines:\n\n\n"},
    {"Mixed newlines: \r\n\r\n", "Mixed newlines: \r\n\r\n"},
    {"Escaped characters: \n\t\r", "Escaped characters: \n\t\r"},
    {"Null character: \0", "Null character: "},

};

INSTANTIATE_TEST_SUITE_P(ConsolePrinterTests, ConsolePrinterTestFixture,
                         ::testing::ValuesIn(console_printer_test_cases));