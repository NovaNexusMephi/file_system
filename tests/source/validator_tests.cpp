#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "command_builder/validator.hpp"

TEST(Validator, ValidateCommand) {
    nlohmann::json j = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": { "segm": 3, "vol": 2, "rec": 10 }
        }
    )");
    Validator validator;
    EXPECT_EQ(validator.validate("init", j), ValidationResult::VALID);
}
