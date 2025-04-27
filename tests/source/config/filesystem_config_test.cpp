#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <libconfig.h++>
#include <string>
#include <vector>
#include "config/filesystem_config.hpp"

using config::FilesystemConfig;

struct FilesystemConfigTestCase {
    std::string volume_id;
    std::string owner_name;
    std::string system_name;
};

class FilesystemConfigTest : public ::testing::TestWithParam<FilesystemConfigTestCase> {
   protected:
    libconfig::Config cfg;
};

TEST_P(FilesystemConfigTest, ParsesFilesystemConfigCorrectly) {
    const auto& [expected_volume_id, expected_owner_name, expected_system_name] = GetParam();

    cfg.getRoot().add("system_info", libconfig::Setting::TypeGroup);
    auto& system_info_setting = cfg.lookup("system_info");
    system_info_setting.add("volume_id", libconfig::Setting::TypeString) = expected_volume_id;
    system_info_setting.add("owner_name", libconfig::Setting::TypeString) = expected_owner_name;
    system_info_setting.add("system_name", libconfig::Setting::TypeString) = expected_system_name;

    FilesystemConfig filesystem_config(cfg.getRoot());

    EXPECT_EQ(filesystem_config.sys_info.volume_id, expected_volume_id);
    EXPECT_EQ(filesystem_config.sys_info.owner_name, expected_owner_name);
    EXPECT_EQ(filesystem_config.sys_info.system_name, expected_system_name);
}

const std::vector<FilesystemConfigTestCase> filesystem_config_test_cases{
    {"vol123", "John Doe", "MySystem"},
    {"vol123", "", "MySystem"},
    {"vol123", "John Doe", ""},
    {"", "John Doe", "MySystem"},
    {"", "", ""},
    {"special!@#$%^&*", "UnicodeName✓", "LongSystemName1234567890"},
    {"vol123", " ", "MySystem"},
    {"vol123", "John Doe", " "},
    {" ", "John Doe", "MySystem"},
    {"vol123", "John_Doe_With_Underscores", "My_System_With_Underscores"},
    {"vol123", "John-Doe-With-Hyphens", "My-System-With-Hyphens"},
    {"vol123", "John.Doe.With.Dots", "My.System.With.Dots"},
    {"vol123", "JohnDoeWithMixedCASE", "MySystemWithMixedCASE"},
    {"vol123", "JohnDoeWithVeryLongName1234567890", "MySystemWithVeryLongName1234567890"},
    {"vol123", "JohnDoe", "MySystemWithSpecialChars!@#$%^&*()"},
    {"vol123", "JohnDoeWithSpecialChars!@#$%^&*()", "MySystem"},
};

INSTANTIATE_TEST_SUITE_P(FilesystemConfigTests, FilesystemConfigTest, testing::ValuesIn(filesystem_config_test_cases));