#include "monitor/reader.h"
#include <gtest/gtest.h>
#include <iomanip>
#include <ios>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

using monitor::Reader;
using json = nlohmann::json;

class ReadTest : public testing::TestWithParam<std::tuple<std::string, std::optional<json>>> {};

TEST_P(ReadTest, HandlesVariousInputs) {
    auto [input, expected] = GetParam();
    std::istringstream stream(input);
    auto result = Reader::read(stream);

    ASSERT_EQ(result.has_value(), expected.has_value());
    if (result && expected) {
        ASSERT_EQ(*result, *expected);
    }
}

// Вспомогательная функция для создания ожидаемого JSON
json create_test_json(const std::string& name, const std::vector<std::string>& data = {},
                      const std::map<std::string, json> options = {}) {
    return {{"name", name}, {"data", data}, {"options", options}};
}

const std::vector<std::tuple<std::string, std::optional<json>>> test_cases{
    // Valid cases
    {"start;", create_test_json("start")},
    {"connect 127.0.0.1:8080;", create_test_json("connect", {"127.0.0.1:8080"})},
    {"update /force;", create_test_json("update", {}, {{"force", true}})},

    // Invalid cases
    {"", std::nullopt},                // Empty stream
    {"no_delimiter", std::nullopt},    // Missing ';'
    {"   ;", std::nullopt},            // Whitespace only
    {";", std::nullopt},               // Empty command
    {"invalid:format;", std::nullopt}  // Invalid command format
};

INSTANTIATE_TEST_SUITE_P(DefaultReadTests, ReadTest, testing::ValuesIn(test_cases));

// Отдельный тест для проверки плохого состояния потока
TEST(ReadTest, HandlesBadStreamState) {
    std::istringstream stream;
    stream.setstate(std::ios::failbit);  // Устанавливаем флаг ошибки

    auto result = Reader::read(stream);
    ASSERT_FALSE(result.has_value());
}

// Тест для проверки чтения нескольких команд
TEST(ReadTest, ReadsMultipleCommands) {
    std::istringstream stream("cmd1;cmd2;");

    auto res1 = Reader::read(stream);
    ASSERT_TRUE(res1);
    ASSERT_EQ(res1->at("name"), "cmd1");

    auto res2 = Reader::read(stream);
    ASSERT_TRUE(res2);
    ASSERT_EQ(res2->at("name"), "cmd2");
}