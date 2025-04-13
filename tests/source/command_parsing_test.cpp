#include <gtest/gtest.h>
#include <string>
#include <tuple>
#include <vector>
#include "gtest/gtest.h"

#include <monitor/command_parsing.h>
using json = nlohmann::json;

class SplitTest : public testing::TestWithParam<std::tuple<std::string, std::string, std::vector<std::string>>> {};

TEST_P(SplitTest, HandlesVariousInputs) {
    const auto& [input, delimiters, expected] = GetParam();
    const auto result = monitor::split(input, delimiters);
    ASSERT_EQ(result, expected);
}

const std::vector<std::tuple<std::string, std::string, std::vector<std::string>>> split_test_cases{
    {"", "", std::vector<std::string>{}},
    {"abc", "", std::vector<std::string>{"abc"}},
    {"a,b,c", ",", {"a", "b", "c"}},
    {"hello world", " ", {"hello", "world"}},
    {"a;b|c", ";|", {"a", "b", "c"}},
    {"test\t\nrest", "\t\n", {"test", "rest"}},
    {"a,,b", ",", {"a", "b"}},
    {",,a,,b,,", ",", {"a", "b"}},
    {"no_delimiters", ",;", {"no_delimiters"}},
    {"all_delimiters", "abcd", {"ll_", "elimiters"}},
    {"mixed  delimiters", " x", {"mi", "ed", "delimiters"}},
    {"dot.separated", ".", {"dot", "separated"}},
    {"a|b|c", "|", {"a", "b", "c"}},
};

INSTANTIATE_TEST_SUITE_P(DefaultSplitTests, SplitTest, testing::ValuesIn(split_test_cases));

class MakeOptTest : public testing::TestWithParam<std::tuple<std::string, std::pair<std::string, std::string>>> {};

TEST_P(MakeOptTest, HandlesVariousOptions) {
    const auto& [input, expected] = GetParam();
    const auto result = monitor::make_opt(input);
    ASSERT_EQ(result, expected);
}

const std::vector<std::tuple<std::string, std::pair<std::string, std::string>>> make_opt_tests_cases{
    {"", {"", ""}},
    {"option", {"", ""}},
    {":value", {"", ""}},
    {"/option", {"option", ""}},
    {"/option:100", {"option", "100"}},
    {"/option:text", {"option", "text"}},
    {"/", {"", ""}},
    {"/:value", {"", "value"}},
    {"/option:", {"option", ""}},
    {"/key:part1:part2", {"key", "part1:part2"}},
};

INSTANTIATE_TEST_SUITE_P(DefaultMakeOptTests, MakeOptTest, ::testing::ValuesIn(make_opt_tests_cases));

class NameParamsTest : public testing::TestWithParam<
                           std::tuple<std::vector<std::string>, std::pair<std::string, std::vector<std::string>>>> {};

TEST_P(NameParamsTest, HandlesVariousInputs) {
    const auto& [input, expected] = GetParam();
    const auto result = monitor::name_params(input);
    ASSERT_EQ(result, expected);
}

const std::vector<std::tuple<std::vector<std::string>, std::pair<std::string, std::vector<std::string>>>>
    name_params_test_cases{
        {{"cmd"}, {"cmd", {}}},
        {{"cmd", "arg1"}, {"cmd", {"arg1"}}},
        {{"cmd", "arg1", "arg2"}, {"cmd", {"arg1", "arg2"}}},
        {{"single"}, {"single", {}}},
        {{"", "param"}, {"", {"param"}}},
        {{"name", ""}, {"name", {""}}},
        {{"composite-name", "value1"}, {"composite-name", {"value1"}}},
        {{"user@mail", "pass#123"}, {"user@mail", {"pass#123"}}},
        {{"action", "1", "2", "3"}, {"action", {"1", "2", "3"}}},
    };

INSTANTIATE_TEST_SUITE_P(DefaultNameParamsTests, NameParamsTest, testing::ValuesIn(name_params_test_cases));

class MakeOptObjTest : public testing::TestWithParam<std::tuple<std::vector<std::string>, json>> {};

TEST_P(MakeOptObjTest, HandlesVariousOptions) {
    const auto& [input, expected] = GetParam();
    const auto result = monitor::make_opt_obj(input);
    ASSERT_EQ(result, expected);
}

const std::vector<std::tuple<std::vector<std::string>, json>> make_opt_obj_test_cases{
    {{}, json::parse("{}")},
    {{"/flag"}, {{"flag", true}}},
    {{"/enable", "/debug"}, {{"enable", true}, {"debug", true}}},
    {{"/timeout:100"}, {{"timeout", 100}}},
    {{"/retries:3"}, {{"retries", 3}}},
    {{"/ratio:255"}, {{"ratio", 255}}},
    {{"/name:John"}, {{"name", "John"}}},
    {{"/message:hello world"}, {{"message", "hello world"}}},
    {{"/enabled", "/count:5", "/title:Test"}, {{"enabled", true}, {"count", 5}, {"title", "Test"}}},
    {{"/user@domain:admin", "/key:123#456"}, {{"user@domain", "admin"}, {"key", "123#456"}}},
    {{"invalid_opt", "/:empty", "/valid:ok"}, {{"valid", "ok"}}},
    {{"/:value"}, json::parse("{}")},
    {{"/empty:"}, {{"empty", true}}},
    {{"/num:0123"}, {{"num", 123}}},
    {{"/verbose", "/force", "/recursive"}, json::parse(R"({"verbose": true, "force": true, "recursive": true})")},
    {{"/setting:5", "/setting:text", "/setting"}, json::parse(R"({"setting": true})")},
    {{"/server:localhost", "/port:8080", "/ssl", "/timeout:30"}, json::parse(R"({
   "server": "localhost",
   "port": 8080,
   "ssl": true,
   "timeout": 30
})")},
    {{"/x:10", "/y:20", "/z:30"}, json::parse(R"({"x": 10, "y": 20, "z": 30})")},
    {{"/valid1", "invalid_opt", "/valid2:ok", "/:empty"}, json::parse(R"({"valid1": true, "valid2": "ok"})")},
};

INSTANTIATE_TEST_SUITE_P(DefaultMakeOptObjTests, MakeOptObjTest, testing::ValuesIn(make_opt_obj_test_cases));

class ParseCommandTest : public testing::TestWithParam<std::tuple<std::string, std::optional<json>>> {};

TEST_P(ParseCommandTest, HandlesVariousCommands) {
    const auto& [input, expected] = GetParam();
    const auto result = monitor::parse_command(input);
    ASSERT_EQ(result.has_value(), expected.has_value());

    if (result && expected) {
        ASSERT_EQ(*result, *expected);
    }
}

const std::vector<std::tuple<std::string, std::optional<json>>> parse_command_test_cases{
    {"", std::nullopt},
    {"start", json::parse(R"({
        "name": "start",
        "data": [],
        "options": {}
    })")},
    {"connect server1 8080", json::parse(R"({
        "name": "connect",
        "data": ["server1", "8080"],
        "options": {}
    })")},
    {"send /async /retries:3", json::parse(R"({
        "name": "send",
        "data": [],
        "options": {
            "async": true,
            "retries": 3
        }
    })")},
    {"update user1 /force /log:verbose", json::parse(R"({
        "name": "update",
        "data": ["user1"],
        "options": {
            "force": true,
            "log": "verbose"
        }
    })")},
    {"execute task1 task2 /priority:5 /background", json::parse(R"({
        "name": "execute",
        "data": ["task1", "task2"],
        "options": {
            "priority": 5,
            "background": true
        }
    })")},
    {"deploy @server /path:/home/user", json::parse(R"({
        "name": "deploy",
        "data": ["@server"],
        "options": {
            "path": "/home/user"
        }
    })")},
    {"/option_only", std::nullopt},
    {"   ", std::nullopt},
    {"\t\n\r", std::nullopt},
    {"stop;server\t/force", json::parse(R"({
        "name": "stop",
        "data": ["server"],
        "options": {
            "force": true
        }
    })")},
    {"process /valid:ok invalid_opt", json::parse(R"({
        "name": "process",
        "data": ["invalid_opt"],
        "options": {
            "valid": "ok"
        }
    })")},
};

INSTANTIATE_TEST_SUITE_P(DefaultParseCommandTests, ParseCommandTest, testing::ValuesIn(parse_command_test_cases));