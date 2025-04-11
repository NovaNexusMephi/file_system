#include <gtest/gtest.h>
#include <monitor/reader.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>

using monitor::Reader;

TEST(ReaderTest, ReadsSimpleCommand) {
    std::istringstream stream("init;");
    Reader reader(stream);

    auto result = reader.read();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->at("command_name"), "init");
    EXPECT_TRUE(result->at("options").empty());
}

TEST(ReaderTest, ReadsNumericOptions) {
    std::istringstream stream("init \\rec:2 \\vol:10;");
    Reader reader(stream);

    auto result = reader.read();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->at("command_name"), "init");
    EXPECT_EQ(result->at("options").at("rec"), 2);
    EXPECT_EQ(result->at("options").at("vol"), 10);
}

TEST(ReaderTest, ReadsStringOptions) {
    std::istringstream stream("play \\name:John \\mode:normal;");
    Reader reader(stream);

    auto result = reader.read();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->at("command_name"), "play");
    EXPECT_EQ(result->at("options").at("name"), "John");
    EXPECT_EQ(result->at("options").at("mode"), "normal");
}

TEST(ReaderTest, ReadsFlags) {
    std::istringstream stream("stop \\force \\verbose;");
    Reader reader(stream);

    auto result = reader.read();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->at("command_name"), "stop");
    EXPECT_TRUE(result->at("options").at("force").is_boolean());
    EXPECT_TRUE(result->at("options").at("verbose").is_boolean());
}

TEST(ReaderTest, ReadsEmptyValues) {
    std::istringstream stream("config \\key:;");
    Reader reader(stream);

    auto result = reader.read();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->at("command_name"), "config");
    EXPECT_EQ(result->at("options").at("key"), true);
}

TEST(ReaderTest, HandlesEmptyString) {
    std::istringstream stream(";");
    Reader reader(stream);

    auto result = reader.read();
    EXPECT_TRUE(result.has_value());
}

TEST(ReaderTest, HandlesMultipleCommands) {
    std::istringstream stream("init \\rec:2; play \\speed:1.5; stop \\force;");
    Reader reader(stream);

    auto result1 = reader.read();
    ASSERT_TRUE(result1.has_value());
    EXPECT_EQ(result1->at("command_name"), "init");
    EXPECT_EQ(result1->at("options").at("rec"), 2);

    auto result2 = reader.read();
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2->at("command_name"), "play");
    EXPECT_EQ(result2->at("options").at("speed"), "1.5");

    auto result3 = reader.read();
    ASSERT_TRUE(result3.has_value());
    EXPECT_EQ(result3->at("command_name"), "stop");
    EXPECT_TRUE(result3->at("options").at("force").is_boolean());

    auto result4 = reader.read();
    EXPECT_FALSE(result4.has_value());
}

TEST(ReaderTest, HandlesMalformedCommand) {
    std::istringstream stream("invalid command;");
    Reader reader(stream);

    auto result = reader.read();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->at("command_name"), "invalid");
    EXPECT_TRUE(result->at("options").empty());
}