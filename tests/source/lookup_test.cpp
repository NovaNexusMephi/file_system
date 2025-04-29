#include <gtest/gtest.h>
#include <config/lookup.hpp>
#include <libconfig.h++>
#include <optional>
#include <string>

struct ConfigTestCase {
    std::string key;
    bool exists;
    std::optional<int> value;
    int default_value;
    bool should_throw;
};

class ConfigTest : public ::testing::TestWithParam<ConfigTestCase> {
   protected:
    libconfig::Config cfg;

    void SetUp() override { cfg.getRoot().add("test", libconfig::Setting::TypeGroup); }
};

TEST_P(ConfigTest, GetFunction) {
    const auto& [key, exists, value, default_value, should_throw] = GetParam();

    auto& test_setting = cfg.lookup("test");
    if (exists) {
        test_setting.add(key, libconfig::Setting::TypeInt) = *value;
    }

    auto result = config::get<int>(test_setting, key);

    if (exists) {
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), *value);
    } else {
        ASSERT_FALSE(result.has_value());
    }
}

TEST_P(ConfigTest, GetDefaultFunction) {
    const auto& [key, exists, value, default_value, should_throw] = GetParam();

    auto& test_setting = cfg.lookup("test");
    if (exists) {
        test_setting.add(key, libconfig::Setting::TypeInt) = *value;
    }

    auto result = config::get_default<int>(test_setting, key, default_value);

    if (exists) {
        EXPECT_EQ(result, *value);
    } else {
        EXPECT_EQ(result, default_value);
    }
}

TEST_P(ConfigTest, GetRequiredFunction) {
    const auto& [key, exists, value, default_value, should_throw] = GetParam();

    auto& test_setting = cfg.lookup("test");
    if (exists) {
        test_setting.add(key, libconfig::Setting::TypeInt) = *value;
    }

    if (should_throw) {
        EXPECT_THROW(config::get_required<int>(test_setting, key), std::runtime_error);
    } else {
        EXPECT_NO_THROW({
            auto result = config::get_required<int>(test_setting, key);
            EXPECT_EQ(result, *value);
        });
    }
}

// Набор тестовых данных
const std::vector<ConfigTestCase> config_test_cases{
    {"existing_key", true, 42, 0, false},           {"missing_key", false, std::nullopt, 10, true},
    {"invalid_type", true, std::nullopt, 0, false}, {"empty_key", false, std::nullopt, 5, true},
    {"negative_value", true, -100, 0, false},       {"zero_value", true, 0, 1, false},
};

// Регистрация тестов
INSTANTIATE_TEST_SUITE_P(ConfigTests, ConfigTest, testing::ValuesIn(config_test_cases));