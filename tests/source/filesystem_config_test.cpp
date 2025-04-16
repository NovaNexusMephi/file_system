#include <gtest/gtest.h>
#include <libconfig.h++>
#include <vector>
#include "gtest/gtest.h"

#include <config/filesystem_config.h>  // Замените на ваш заголовочный файл
using config::FilesystemConfig;

using namespace libconfig;

struct FilesystemConfigTestCase {
    std::string description;
    std::string config_data;
    bool should_throw;
    std::string expected_exception_message;
    std::string expected_volume_id;
    std::string expected_owner_name;
    std::string expected_system_name;
};

class FilesystemConfigTest : public ::testing::TestWithParam<FilesystemConfigTestCase> {
   protected:
    libconfig::Config cfg;

    void SetUp() override {
        const auto& param = GetParam();
        if (!param.config_data.empty()) {
            cfg.readString(param.config_data);
        }
    }
};

TEST_P(FilesystemConfigTest, ConstructorValidation) {
    const auto& param = GetParam();

    if (param.should_throw) {
        EXPECT_THROW(FilesystemConfig config(cfg.lookup("")), std::runtime_error);
    } else {
        FilesystemConfig config(cfg.lookup(""));
        EXPECT_EQ(config.sys_info.volume_id, param.expected_volume_id);
        EXPECT_EQ(config.sys_info.owner_name, param.expected_owner_name);
        EXPECT_EQ(config.sys_info.system_name, param.expected_system_name);
    }
}

const std::vector<FilesystemConfigTestCase> fs_cfg_test_cases = {
    {
        "ValidConfiguration",
        R"(
        filesystem: {
            system_info: {
                volume_id = "VOL001";
                owner_name = "admin";
                system_name = "FS_Manager";
            };
        };
        )",
        false,
        "",
        "VOL001",
        "admin",
        "FS_Manager",
    },
};

INSTANTIATE_TEST_SUITE_P(FilesystemConfigTests, FilesystemConfigTest, ::testing::ValuesIn(fs_cfg_test_cases),
                         [](const ::testing::TestParamInfo<FilesystemConfigTestCase>& info) {
                             return info.param.description;
                         });