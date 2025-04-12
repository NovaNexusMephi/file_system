#include "monitor/reader.h"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <sstream>
#include <string>
#include <vector>
#include "gtest/gtest.h"

using json = nlohmann::json;
using monitor::Reader;

struct ReaderTestCase {
    std::string input{};
    bool has_value{};
    nlohmann::json expected{};
};

const std::vector<ReaderTestCase> test_cases = {
    {"DIR;", true, R"({"name": "DIR", "data": [], "options": {}})"_json},
    {"DIR /FULL;", true, R"({"name": "DIR", "data": [], "options": {"FULL": true}})"_json},
    {"DIR /FULL /OUTPUT:file.txt;", true,
     R"({"name": "DIR", "data": [], "options": {"FULL": true, "OUTPUT":"file.txt"}})"_json},
    {"CREATE file.txt /ALLOCATE:10", true,
     R"({"name": "CREATE", "data": ["file.txt"], "options": {"ALLOCATE": 10}})"_json},

};

class ReaderTest : public ::testing::TestWithParam<ReaderTestCase> {};

TEST_P(ReaderTest, Read) {
    Reader reader{};
    auto [input, has_value, expected] = GetParam();
    std::istringstream ss{input};
    auto actual = reader.read(ss);
    ASSERT_EQ(actual.has_value(), has_value);
    ASSERT_EQ(*actual, expected);
}

INSTANTIATE_TEST_SUITE_P(ReaderTests, ReaderTest, ::testing::ValuesIn(test_cases));