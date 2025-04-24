#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include <monitor/scanners/file_scanner.hpp>
#include <string>
#include <vector>
#include "gtest/gtest.h"
using monitor::scanners::FileScanner;

struct FileScannerTestParams {
    std::string description;
    std::string input;
    std::vector<std::string> expected;
};

class FileScannerTestFixture : public ::testing::TestWithParam<FileScannerTestParams> {
   protected:
    void SetUp() override {
        temp_dir_ = std::filesystem::temp_directory_path();
        temp_file_path_ = temp_dir_ / std::filesystem::path("test_file_XXXXX.txt");
        std::ofstream temp_file(temp_file_path_);
        temp_file << GetParam().input;
    }

    void TearDown() override { std::filesystem::remove(temp_file_path_); }

    std::filesystem::path temp_file_path_;
    std::filesystem::path temp_dir_;
};

TEST_P(FileScannerTestFixture, NextReturnsExpectedLines) {
    FileScanner scanner(temp_file_path_);
    const auto& expected_lines = GetParam().expected;

    for (const auto& expected_line : expected_lines) {
        ASSERT_TRUE(scanner.has_next()) << "Failed at line: " << expected_line;
        EXPECT_EQ(scanner.next(), expected_line);
    }
    EXPECT_FALSE(scanner.has_next()) << "Expected no more lines";
}

const std::vector<FileScannerTestParams> file_scanner_tests_cases{
    FileScannerTestParams{"Multiply lines input 1", "Line1;\nLine2;", std::vector<std::string>{"Line1", "Line2"}},
    FileScannerTestParams{"Multiply lines input 2", "Line1;\nLine2;\nLine3;",
                          std::vector<std::string>{"Line1", "Line2", "Line3"}},
    FileScannerTestParams{"Multiply lines input 4", "Line1;\nLine2;\nLine3;\nLine4;",
                          std::vector<std::string>{"Line1", "Line2", "Line3", "Line4"}},
    FileScannerTestParams{"Single line input", "Line1;", std::vector<std::string>{"Line1"}},
    FileScannerTestParams{"Empty input", "", std::vector<std::string>{}},
};

INSTANTIATE_TEST_SUITE_P(FileScannerTests, FileScannerTestFixture, ::testing::ValuesIn(file_scanner_tests_cases));