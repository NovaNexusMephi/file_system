#include <gtest/gtest.h>

#include <monitor/parser.hpp>
using namespace monitor;

class ParserTest : public ::testing::Test {};

struct ParseTestCase {
    std::string input;
    bool expect_success;
    nlohmann::json expected_output;
};

class ParseTest : public ::testing::TestWithParam<ParseTestCase> {};

TEST_P(ParseTest, ParsesCommandLineCorrectly) {
    auto param = GetParam();
    auto result = Parser::parse(param.input);

    if (param.expect_success) {
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(*result, param.expected_output);
    } else {
        EXPECT_FALSE(result.has_value());
    }
}

static const std::vector<ParseTestCase> parse_test_cases = {
    // Existing test cases
    {"command arg1 arg2 /opt1:value1 /opt2", true,
     R"({
            "name": "command",
            "data": ["arg1", "arg2"],
            "options": {
                "opt1": "value1",
                "opt2": true
            }
        })"_json},
    {"run /debug /timeout:30", true,
     R"({
            "name": "run",
            "data": [],
            "options": {
                "debug": true,
                "timeout": 30
            }
        })"_json},
    {"test arg1 /key:value /num:42", true,
     R"({
            "name": "test",
            "data": ["arg1"],
            "options": {
                "key": "value",
                "num": 42
            }
        })"_json},
    {"", false, {}},
    {"   ", false, {}},
    {"/opt1:value1 /opt2", false, {}},
    {"start /verbose /level:5", true,
     R"({
            "name": "start",
            "data": [],
            "options": {
                "verbose": true,
                "level": 5
            }
        })"_json},
    {"deploy app /env:prod /force", true,
     R"({
            "name": "deploy",
            "data": ["app"],
            "options": {
                "env": "prod",
                "force": true
            }
        })"_json},
    {"build /output:bin /optimize", true,
     R"({
            "name": "build",
            "data": [],
            "options": {
                "output": "bin",
                "optimize": true
            }
        })"_json},
    {"execute script.js /mode:dev /workers:4", true,
     R"({
            "name": "execute",
            "data": ["script.js"],
            "options": {
                "mode": "dev",
                "workers": 4
            }
        })"_json},
    {"configure /path:/usr/local /overwrite", true,
     R"({
            "name": "configure",
            "data": [],
            "options": {
                "path": "/usr/local",
                "overwrite": true
            }
        })"_json},
    {"init project /template:default /skip-install", true,
     R"({
            "name": "init",
            "data": ["project"],
            "options": {
                "template": "default",
                "skip-install": true
            }
        })"_json},
    {"monitor /interval:10 /log:debug", true,
     R"({
            "name": "monitor",
            "data": [],
            "options": {
                "interval": 10,
                "log": "debug"
            }
        })"_json},
    {"upload file.txt /destination:/remote/path /overwrite", true,
     R"({
            "name": "upload",
            "data": ["file.txt"],
            "options": {
                "destination": "/remote/path",
                "overwrite": true
            }
        })"_json},
    {"download /source:/remote/file.txt /destination:/local/path", true,
     R"({
            "name": "download",
            "data": [],
            "options": {
                "source": "/remote/file.txt",
                "destination": "/local/path"
            }
        })"_json},
    {"list /type:files /recursive", true,
     R"({
            "name": "list",
            "data": [],
            "options": {
                "type": "files",
                "recursive": true
            }
        })"_json},
    {"search query /limit:100 /sort:asc", true,
     R"({
            "name": "search",
            "data": ["query"],
            "options": {
                "limit": 100,
                "sort": "asc"
            }
        })"_json},
    {"backup /source:/data /target:/backup /compress", true,
     R"({
            "name": "backup",
            "data": [],
            "options": {
                "source": "/data",
                "target": "/backup",
                "compress": true
            }
        })"_json},
    {"restore /source:/backup /target:/data /overwrite", true,
     R"({
            "name": "restore",
            "data": [],
            "options": {
                "source": "/backup",
                "target": "/data",
                "overwrite": true
            }
        })"_json},
    {"validate config.json /strict", true,
     R"({
            "name": "validate",
            "data": ["config.json"],
            "options": {
                "strict": true
            }
        })"_json},
    {"generate report /format:pdf /pages:10", true,
     R"({
            "name": "generate",
            "data": ["report"],
            "options": {
                "format": "pdf",
                "pages": 10
            }
        })"_json},
};

INSTANTIATE_TEST_SUITE_P(CommandParserTests, ParseTest, ::testing::ValuesIn(parse_test_cases));