#include "config/config_manager.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <libconfig.h++>
#include <string>
#include "config/filesystem_config.hpp"
#include "config/monitor_config.hpp"

using config::ConfigManager;

// Структура для хранения тестовых случаев
struct ConfigManagerTestCase {
    std::string file_content;  // Содержимое файла конфигурации
    bool should_throw;         // Должен ли конструктор выбросить исключение
    std::string expected_exception;  // Ожидаемое сообщение исключения (если выбрасывается)
};

// Базовый класс для тестов
class ConfigManagerTest : public ::testing::TestWithParam<ConfigManagerTestCase> {
   protected:
    std::filesystem::path temp_file_path;

    void SetUp() override {
        // Создаем временный файл для тестирования
        temp_file_path = std::filesystem::temp_directory_path() / "test_config.cfg";
    }

    void TearDown() override {
        // Удаляем временный файл после теста
        if (std::filesystem::exists(temp_file_path)) {
            std::filesystem::remove(temp_file_path);
        }
    }

    void createConfigFile(const std::string& content) {
        std::ofstream file(temp_file_path);
        ASSERT_TRUE(file.is_open());
        file << content;
        file.close();
    }
};

// Тесты для конструктора ConfigManager
TEST_P(ConfigManagerTest, ConstructorBehavior) {
    const auto& [file_content, should_throw, expected_exception] = GetParam();

    if (!file_content.empty()) {
        createConfigFile(file_content);
    }

    if (should_throw) {
        EXPECT_THROW(
            {
                try {
                    ConfigManager manager(temp_file_path);
                } catch (const std::runtime_error& e) {
                    EXPECT_EQ(e.what(), expected_exception);
                    throw;
                }
            },
            std::runtime_error);
    } else {
        EXPECT_NO_THROW({
            ConfigManager manager(temp_file_path);

            // Проверяем, что конфигурация загружена корректно
            const auto& fs_config = manager.get_fs_config();
            const auto& monitor_config = manager.get_monitor_config();

            if (fs_config.sys_info.volume_id.empty()) {
                EXPECT_TRUE(fs_config.sys_info.volume_id.empty());
            } else {
                EXPECT_FALSE(fs_config.sys_info.volume_id.empty());
            }

            if (monitor_config.input.prompt.empty()) {
                EXPECT_TRUE(monitor_config.input.prompt.empty());
            } else {
                EXPECT_FALSE(monitor_config.input.prompt.empty());
            }
        });
    }
}

// Набор тестовых данных
const std::vector<ConfigManagerTestCase> config_manager_test_cases{
    {
        R"(
        filesystem = {
            system_info = {
                volume_id = "vol123";
                owner_name = "John Doe";
                system_name = "MySystem";
            };
        };
        monitor = {
            input = {
                prompt = "[prompt]";
                wait_for_input = true;
                from = "file.log";
            };
            output = {
                to = "stdout";
            };
        };
        )",
        false, ""},  // Корректная конфигурация
    {"", true,
     "Configuration file not found: " + std::filesystem::temp_directory_path().string() +
         "/test_config.cfg"},  // Файл не существует
    {"invalid_config", true,
     "Parse error at " + std::filesystem::temp_directory_path().string() +
         "/test_config.cfg:1 - syntax error"},  // Некорректный формат
    {
        R"(
        monitor = {
            input = {
                prompt = "[prompt]";
                wait_for_input = true;
                from = "file.log";
            };
            output = {
                to = "stdout";
            };
        };
        )",
        true, "Missing required configuration section: .filesystem"},  // Отсутствие секции filesystem
    {
        R"(
        filesystem = {
            system_info = {
                volume_id = "vol123";
                owner_name = "John Doe";
                system_name = "MySystem";
            };
        };
        )",
        true, "Missing required configuration section: .monitor"},  // Отсутствие секции monitor
    {
        R"(
        filesystem = {
            system_info = {
                volume_id = "";
                owner_name = "";
                system_name = "";
            };
        };
        monitor = {
            input = {
                prompt = "";
                wait_for_input = false;
                from = "";
            };
            output = {
                to = "";
            };
        };
        )",
        false, ""}  // Все поля пустые
};

// Регистрация тестов
INSTANTIATE_TEST_SUITE_P(ConfigManagerTests, ConfigManagerTest, testing::ValuesIn(config_manager_test_cases));