#include <gtest/gtest.h>
#include <libconfig.h++>
#include <memory>
#include <optional>
#include <vector>
#include "gtest/gtest.h"

#include <config/lookup.h>

using config::get;
using config::get_default;
using config::get_required;

#include "lookup_helpers.h"

template <typename T>
class GetTypedTest : public testing::Test {
   protected:
    std::unique_ptr<libconfig::Config> cfg{};
    libconfig::Setting* root{nullptr};

    void SetUp() override {
        cfg = std::make_unique<libconfig::Config>();
        root = &cfg->getRoot();
    }

    void add_value(const std::string& name, T value) { helpers::add_setting(*root, name, value); }
};

TYPED_TEST_SUITE(GetTypedTest, LookupTestTypes);

TYPED_TEST(GetTypedTest, ReturnsValueWhenKeyExists) {
    TypeParam test_value{};
    const std::string key = "test_key";

    if constexpr (std::is_same_v<TypeParam, int>) {
        test_value = 42;
    } else if constexpr (std::is_same_v<TypeParam, double>) {
        test_value = 3.14;
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
        test_value = "test_string";
    } else if constexpr (std::is_same_v<TypeParam, bool>) {
        test_value = true;
    }

    this->add_value(key, test_value);
    auto result = get<TypeParam>(*this->root, key);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), test_value);
}

TYPED_TEST(GetTypedTest, ReturnsNulloptWhenKeyMissing) {
    const std::string key = "non_existing_key";
    auto result = get<TypeParam>(*this->root, key);
    EXPECT_FALSE(result.has_value());
}

TYPED_TEST(GetTypedTest, ReturnsValueWhenKeyExists_Default) {
    TypeParam test_value{};
    const std::string key = "test_key";
    const TypeParam default_value = TypeParam{};

    if constexpr (std::is_same_v<TypeParam, int>) {
        test_value = 42;
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
        test_value = "test_string";
    } else if constexpr (std::is_same_v<TypeParam, bool>) {
        test_value = true;
    }

    this->add_value(key, test_value);
    auto result = get_default(*this->root, key, default_value);

    EXPECT_EQ(result, test_value);
}

TYPED_TEST(GetTypedTest, ReturnsDefaultWhenKeyMissing_Default) {
    const std::string key = "non_existing_key";
    const TypeParam default_value = TypeParam{};

    auto result = get_default(*this->root, key, default_value);
    EXPECT_EQ(result, default_value);
}

struct TypeTestCase {
    std::string description;
    std::string key;
    libconfig::Setting::Type type;
    std::optional<int> int_val;
    std::optional<std::string> str_val;
    std::optional<bool> bool_val;
};

class GetTypeSafetyTest : public testing::TestWithParam<TypeTestCase> {
   protected:
    std::unique_ptr<libconfig::Config> config;
    libconfig::Setting* root;

    void SetUp() override {
        config = std::make_unique<libconfig::Config>();
        root = &config->getRoot();
        const auto& param = GetParam();

        switch (param.type) {
            case libconfig::Setting::TypeInt:
                helpers::add_setting<int>(*root, param.key, param.int_val.value());
                break;
            case libconfig::Setting::TypeString:
                helpers::add_setting<std::string>(*root, param.key, param.str_val.value());
                break;
            case libconfig::Setting::TypeBoolean:
                helpers::add_setting<bool>(*root, param.key, param.bool_val.value());
                break;
            default:
                break;
        }
    }
};

TEST_P(GetTypeSafetyTest, CorrectlyHandlesTypes) {
    const auto& param = GetParam();

    EXPECT_EQ(get<int>(*root, param.key).has_value(), param.type == libconfig::Setting::TypeInt);

    EXPECT_EQ(get<std::string>(*root, param.key).has_value(), param.type == libconfig::Setting::TypeString);

    EXPECT_EQ(get<bool>(*root, param.key).has_value(), param.type == libconfig::Setting::TypeBoolean);
}

const std::vector<TypeTestCase> type_test_cases = {
    {"Integer", "int_key", libconfig::Setting::TypeInt, 42, {}, {}},
    {"String", "str_key", libconfig::Setting::TypeString, {}, {"test"}, {}},
    {"Boolean", "bool_key", libconfig::Setting::TypeBoolean, {}, {}, true},
};

INSTANTIATE_TEST_SUITE_P(TypeSafety, GetTypeSafetyTest, testing::ValuesIn(type_test_cases),
                         [](const testing::TestParamInfo<TypeTestCase>& info) { return info.param.description; });
struct DefaultTestCase {
    std::string description;
    std::string key;
    libconfig::Setting::Type type;
    std::optional<int> int_val;
    std::optional<std::string> str_val;
    std::optional<bool> bool_val;
    int default_int;
    std::string default_str;
    bool default_bool;
    int expected_int;
    std::string expected_str;
    bool expected_bool;
};

class GetDefaultTest : public testing::TestWithParam<DefaultTestCase> {
   protected:
    std::unique_ptr<libconfig::Config> config;
    libconfig::Setting* root;

    void SetUp() override {
        config = std::make_unique<libconfig::Config>();
        root = &config->getRoot();
        const auto& param = GetParam();

        if (param.type != libconfig::Setting::TypeNone) {
            switch (param.type) {
                case libconfig::Setting::TypeInt:
                    helpers::add_setting<int>(*root, param.key, param.int_val.value());
                    break;
                case libconfig::Setting::TypeString:
                    helpers::add_setting<std::string>(*root, param.key, param.str_val.value());
                    break;
                case libconfig::Setting::TypeBoolean:
                    helpers::add_setting<bool>(*root, param.key, param.bool_val.value());
                    break;
                default:
                    break;
            }
        }
    }
};

TEST_P(GetDefaultTest, ReturnsCorrectDefaultValues) {
    const auto& param = GetParam();

    int default_int = param.default_int;
    std::string default_str = param.default_str;
    bool default_bool = param.default_bool;

    EXPECT_EQ(get_default(*root, param.key, default_int), param.expected_int);
    EXPECT_EQ(get_default(*root, param.key, default_str), param.expected_str);
    EXPECT_EQ(get_default(*root, param.key, default_bool), param.expected_bool);
}

const std::vector<DefaultTestCase> default_test_cases = {
    {"ExistingInt_ReturnsValue",
     "int_key",
     libconfig::Setting::TypeInt,
     42,
     {},
     {},
     0,
     "default",
     false,
     42,
     "default",
     false},
    {"ExistingString_ReturnsValue",
     "str_key",
     libconfig::Setting::TypeString,
     {},
     "test",
     {},
     0,
     "default",
     false,
     0,
     "test",
     false},
    {"ExistingBool_ReturnsValue",
     "bool_key",
     libconfig::Setting::TypeBoolean,
     {},
     {},
     true,
     0,
     "default",
     false,
     0,
     "default",
     true},
    {"MissingKey_ReturnsDefault",
     "missing_key",
     libconfig::Setting::TypeNone,
     {},
     {},
     {},
     999,
     "default",
     true,
     999,
     "default",
     true},
};

INSTANTIATE_TEST_SUITE_P(DefaultSafety, GetDefaultTest, testing::ValuesIn(default_test_cases),
                         [](const testing::TestParamInfo<DefaultTestCase>& info) { return info.param.description; });

struct RequiredTestCase {
    std::string description;
    std::string key;
    libconfig::Setting::Type type;
    std::optional<int> int_val;
    std::optional<std::string> str_val;
    std::optional<bool> bool_val;
    bool should_throw_type_error;
    bool should_throw_missing_error;
};

class GetRequiredTest : public testing::TestWithParam<RequiredTestCase> {
   protected:
    std::unique_ptr<libconfig::Config> config;
    libconfig::Setting* root;

    void SetUp() override {
        config = std::make_unique<libconfig::Config>();
        root = &config->getRoot();
        const auto& param = GetParam();

        if (param.type != libconfig::Setting::TypeNone) {
            switch (param.type) {
                case libconfig::Setting::TypeInt:
                    helpers::add_setting<int>(*root, param.key, param.int_val.value());
                    break;
                case libconfig::Setting::TypeString:
                    helpers::add_setting<std::string>(*root, param.key, param.str_val.value());
                    break;
                case libconfig::Setting::TypeBoolean:
                    helpers::add_setting<bool>(*root, param.key, param.bool_val.value());
                    break;
                default:
                    break;
            }
        }
    }
};

TEST_P(GetRequiredTest, HandlesRequiredValues) {
    const auto& param = GetParam();

    if (param.should_throw_missing_error) {
        EXPECT_THROW(get_required<int>(*root, param.key), std::runtime_error);
        EXPECT_THROW(get_required<std::string>(*root, param.key), std::runtime_error);
        EXPECT_THROW(get_required<bool>(*root, param.key), std::runtime_error);
    } else if (param.should_throw_type_error) {
        EXPECT_THROW(get_required<int>(*root, param.key), std::runtime_error);
        EXPECT_THROW(get_required<std::string>(*root, param.key), std::runtime_error);
        EXPECT_THROW(get_required<bool>(*root, param.key), std::runtime_error);
    } else {
        switch (param.type) {
            case libconfig::Setting::TypeInt:
                EXPECT_NO_THROW(EXPECT_EQ(get_required<int>(*root, param.key), param.int_val.value()));
                break;
            case libconfig::Setting::TypeString:
                EXPECT_NO_THROW(EXPECT_EQ(get_required<std::string>(*root, param.key), param.str_val.value()));
                break;
            case libconfig::Setting::TypeBoolean:
                EXPECT_NO_THROW(EXPECT_EQ(get_required<bool>(*root, param.key), param.bool_val.value()));
                break;
            default:
                break;
        }
    }
}

const std::vector<RequiredTestCase> required_test_cases = {
    {"ExistingInt_ReturnsValue", "int_key", libconfig::Setting::TypeInt, 42, {}, {}, false, false},
    {"ExistingString_ReturnsValue", "str_key", libconfig::Setting::TypeString, {}, "test", {}, false, false},
    {"ExistingBool_ReturnsValue", "bool_key", libconfig::Setting::TypeBoolean, {}, {}, true, false, false},
    {"MissingKey_ThrowsMissingError", "missing_key", libconfig::Setting::TypeNone, {}, {}, {}, false, true},
};

INSTANTIATE_TEST_SUITE_P(RequiredSafety, GetRequiredTest, testing::ValuesIn(required_test_cases),
                         [](const testing::TestParamInfo<RequiredTestCase>& info) { return info.param.description; });