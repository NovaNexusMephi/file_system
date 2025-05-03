#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "command_builder/validator.hpp"

using command_builder::ValidationResult;
using command_builder::Validator;

TEST(Validator, DirCommandValid) {
    Validator validator;
    nlohmann::json regular = nlohmann::json::parse(R"(
        {
            "name": "dir",
            "data": [],
            "options": {}
        }
    )");

    nlohmann::json full = nlohmann::json::parse(R"(
        {
            "name": "dir",
            "data": [ "full" ],
            "options": {}
        }
    )");

    EXPECT_EQ(validator.validate("dir", regular), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("dir", full), ValidationResult::VALID);
}

TEST(Validator, DirCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "dir",
            "data": [ "full", "full" ],
            "options": {}
        }
    )");

    nlohmann::json data_invalid = nlohmann::json::parse(R"(
        {
            "name": "dir",
            "data": [ "fill" ],
            "options": {}
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "dir",
            "data": [],
            "options": { "full": true }
        }
    )");

    nlohmann::json options_excessive_with_data = nlohmann::json::parse(R"(
        {
            "name": "dir",
            "data": ["full"],
            "options": { "full": true }
        }
    )");

    nlohmann::json data_ivalid_type = nlohmann::json::parse(R"(
        {
            "name": "dir",
            "data": [ 1 ],
            "options": {}
        }
    )");

    EXPECT_EQ(validator.validate("dir", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("dir", data_invalid), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("dir", options_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("dir", options_excessive_with_data), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("dir", data_ivalid_type), ValidationResult::INVALID);
}

TEST(Validator, InitCommand) {
    Validator validator;
    nlohmann::json j = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": { "segm": 3, "vol": 2, "rec": 10 }
        }
    )");
    EXPECT_EQ(validator.validate("init", j), ValidationResult::VALID);
}

TEST(Validator, InitCommandInvalid) {
    Validator validator;
    nlohmann::json data_insufficient = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [],
            "options": { "segm": 3, "vol": 2, "rec": 10 }
        }
    )");

    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER", "VOL" ],
            "options": { "segm": 3, "vol": 2, "rec": 10 }
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": { "segm": 3, "vol": 2, "rec": 10, "full": true }
        }
    )");

    nlohmann::json options_insufficient = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": {}
        }
    )");

    nlohmann::json option_invalid_segm = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": { "segm": 0, "vol": 2, "rec": 10 }
        }
    )");

    nlohmann::json option_invalid_vol = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": { "segm": 3, "vol": 0, "rec": 10 }
        }
    )");

    nlohmann::json option_invalid_rec = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": { "segm": 3, "vol": 2, "rec": 0 }
        }
    )");

    nlohmann::json option_invalid_segm_rec = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": { "segm": 0, "vol": 2, "rec": 0 }
        }
    )");

    nlohmann::json option_invalid_vol_rec = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": { "segm": 3, "vol": 0, "rec": 0 }
        }
    )");

    nlohmann::json option_invalid_segm_vol = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": { "segm": 0, "vol": 2, "rec": 10 }
        }
    )");

    nlohmann::json option_invalid_segm_vol_rec = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": { "segm": 0, "vol": 0, "rec": 0 }
        }
    )");

    nlohmann::json data_invalid_type = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ 1 ],
            "options": { "segm": 3, "vol": 2, "rec": 10 }
        }
    )");

    nlohmann::json options_invalid_type = nlohmann::json::parse(R"(
        {
            "name": "init",
            "data": [ "VOL", "OWNER" ],
            "options": { "segm": "3", "vol": "2", "rec": "10" }
        }
    )");

    EXPECT_EQ(validator.validate("init", data_insufficient), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", options_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", options_insufficient), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", option_invalid_segm), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", option_invalid_vol), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", option_invalid_rec), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", option_invalid_segm_rec), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", option_invalid_vol_rec), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", option_invalid_segm_vol), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", option_invalid_segm_vol_rec), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", data_invalid_type), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("init", options_invalid_type), ValidationResult::INVALID);
}

TEST(Validator, CreateCommandValid) {
    Validator validator;
    nlohmann::json create_valid = nlohmann::json::parse(R"(
        {
            "name": "create",
            "data": [ "f1.txt" ],
            "options": { "allocate": 10 }
        }
    )");

    EXPECT_EQ(validator.validate("create", create_valid), ValidationResult::VALID);
}

TEST(Validator, CreateCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "create",
            "data": [ "f1.txt", "f2.txt" ],
            "options": { "allocate": 10 }
        }
    )");

    nlohmann::json data_insufficient = nlohmann::json::parse(R"(
        {
            "name": "create",
            "data": [],
            "options": { "allocate": 10 }
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "create",
            "data": [ "f1.txt" ],
            "options": { "allocate": 10, "full": true }
        }
    )");

    nlohmann::json options_insufficient = nlohmann::json::parse(R"(
        {
            "name": "create",
            "data": [ "f1.txt" ],
            "options": {}
        }
    )");

    nlohmann::json options_invalid = nlohmann::json::parse(R"(
        {
            "name": "create",
            "data": [ "f1.txt" ],
            "options": { "allocate": 0 }
        }
    )");

    nlohmann::json data_invalid_type = nlohmann::json::parse(R"(
        {
            "name": "create",
            "data": [ 1 ],
            "options": { "allocate": 10 }
        }
    )");

    nlohmann::json options_invalid_type = nlohmann::json::parse(R"(
        {
            "name": "create",
            "data": [ "f1.txt" ],
            "options": { "allocate": "10" }
        }
    )");

    EXPECT_EQ(validator.validate("create", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("create", data_insufficient), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("create", options_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("create", options_insufficient), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("create", options_invalid), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("create", data_invalid_type), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("create", options_invalid_type), ValidationResult::INVALID);
}

TEST(Validator, DeleteCommandValid) {
    Validator validator;
    nlohmann::json delete_valid = nlohmann::json::parse(R"(
        {
            "name": "delete",
            "data": [ "f1.txt" ],
            "options": {}
        }
    )");
    EXPECT_EQ(validator.validate("delete", delete_valid), ValidationResult::VALID);
}

TEST(Validator, DeleteCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "delete",
            "data": [ "f1.txt", "f2.txt" ],
            "options": {}
        }
    )");

    nlohmann::json data_insufficient = nlohmann::json::parse(R"(
        {
            "name": "delete",
            "data": [],
            "options": {}
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "delete",
            "data": [ "f1.txt" ],
            "options": { "full": true }
        }
    )");

    nlohmann::json data_invalid_type = nlohmann::json::parse(R"(
        {
            "name": "delete",
            "data": [ 1 ],
            "options": {}
        }
    )");

    EXPECT_EQ(validator.validate("delete", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("delete", data_insufficient), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("delete", options_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("delete", data_invalid_type), ValidationResult::INVALID);
}

TEST(Validator, RenameCommandValid) {
    Validator validator;
    nlohmann::json rename_valid = nlohmann::json::parse(R"(
        {
            "name": "rename",
            "data": [ "f1.txt", "f2.txt" ],
            "options": {}
        }
    )");
    EXPECT_EQ(validator.validate("rename", rename_valid), ValidationResult::VALID);
}

TEST(Validator, RenameCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "rename",
            "data": [ "f1.txt", "f2.txt", "f3.txt" ],
            "options": {}
        }
    )");

    nlohmann::json data_insufficient = nlohmann::json::parse(R"(
        {
            "name": "rename",
            "data": [],
            "options": {}
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "rename",
            "data": [ "f1.txt", "f2.txt" ],
            "options": { "full": true }
        }
    )");

    nlohmann::json data_invalid_type = nlohmann::json::parse(R"(
        {
            "name": "rename",
            "data": [ 1, 2 ],
            "options": {}
        }
    )");

    EXPECT_EQ(validator.validate("rename", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("rename", data_insufficient), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("rename", options_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("rename", data_invalid_type), ValidationResult::INVALID);
}

TEST(Validator, CopyCommandValid) {
    Validator validator;
    nlohmann::json copy_valid = nlohmann::json::parse(R"(
        {
            "name": "copy",
            "data": [ "f1.txt", "f2.txt" ],
            "options": {}
        }
    )");
    EXPECT_EQ(validator.validate("copy", copy_valid), ValidationResult::VALID);
}

TEST(Validator, CopyCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "copy",
            "data": [ "f1.txt", "f2.txt", "f3.txt" ],
            "options": {}
        }
    )");

    nlohmann::json data_insufficient = nlohmann::json::parse(R"(
        {
            "name": "copy",
            "data": [],
            "options": {}
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "copy",
            "data": [ "f1.txt", "f2.txt" ],
            "options": { "full": true }
        }
    )");

    nlohmann::json data_invalid_type = nlohmann::json::parse(R"(
        {
            "name": "copy",
            "data": [ 1, 2 ],
            "options": {}
        }
    )");

    EXPECT_EQ(validator.validate("copy", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("copy", data_insufficient), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("copy", options_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("copy", data_invalid_type), ValidationResult::INVALID);
}

TEST(Validator, MoveCommandValid) {
    Validator validator;
    nlohmann::json move_valid = nlohmann::json::parse(R"(
        {
            "name": "move",
            "data": [ "f1.txt", "f2.txt" ],
            "options": {}
        }
    )");
    EXPECT_EQ(validator.validate("move", move_valid), ValidationResult::VALID);
}

TEST(Validator, MoveCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "move",
            "data": [ "f1.txt", "f2.txt", "f3.txt" ],
            "options": {}
        }
    )");

    nlohmann::json data_insufficient = nlohmann::json::parse(R"(
        {
            "name": "move",
            "data": [],
            "options": {}
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "move",
            "data": [ "f1.txt", "f2.txt" ],
            "options": { "full": true }
        }
    )");

    nlohmann::json data_invalid_type = nlohmann::json::parse(R"(
        {
            "name": "move",
            "data": [ 1, 2 ],
            "options": {}
        }
    )");

    EXPECT_EQ(validator.validate("move", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("move", data_insufficient), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("move", options_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("move", data_invalid_type), ValidationResult::INVALID);
}

TEST(Validator, AddCommandValid) {
    Validator validator;
    nlohmann::json add_valid = nlohmann::json::parse(R"(
        {
            "name": "add",
            "data": [ "f1.txt" ],
            "options": { "size": 10 }
        }
    )");
    EXPECT_EQ(validator.validate("add", add_valid), ValidationResult::VALID);
}

TEST(Validator, AddCommandInvalid) {
    Validator validator;
    nlohmann::json data_insufficient = nlohmann::json::parse(R"(
        {
            "name": "add",
            "data": [],
            "options": { "size": 10 }
        }
    )");

    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "add",
            "data": [ "f1.txt", "f2.txt" ],
            "options": { "size": 10 }
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "add",
            "data": [ "f1.txt" ],
            "options": { "size": 10, "full": true }
        }
    )");

    nlohmann::json options_insufficient = nlohmann::json::parse(R"(
        {
            "name": "add",
            "data": [ "f1.txt" ],
            "options": {}
        }
    )");

    nlohmann::json option_invalid_size = nlohmann::json::parse(R"(
        {
            "name": "add",
            "data": [ "f1.txt" ],
            "options": { "size": 0 }
        }
    )");

    nlohmann::json data_invalid_type = nlohmann::json::parse(R"(
        {
            "name": "add",
            "data": [ 1 ],
            "options": { "size": 10 }
        }
    )");

    nlohmann::json options_invalid_type = nlohmann::json::parse(R"(
        {
            "name": "add",
            "data": [ "f1.txt" ],
            "options": { "size": "10" }
        }
    )");

    EXPECT_EQ(validator.validate("add", data_insufficient), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("add", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("add", options_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("add", options_insufficient), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("add", option_invalid_size), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("add", data_invalid_type), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("add", options_invalid_type), ValidationResult::INVALID);
}

TEST(Validator, SqueezeCommandValid) {
    Validator validator;
    nlohmann::json squeeze_valid = nlohmann::json::parse(R"(
        {
            "name": "squeeze",
            "data": [],
            "options": {}
        }
    )");
    EXPECT_EQ(validator.validate("squeeze", squeeze_valid), ValidationResult::VALID);
}

TEST(Validator, SqueezeCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "squeeze",
            "data": [ "f1.txt" ],
            "options": {}
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "squeeze",
            "data": [],
            "options": { "full": true }
        }
    )");

    EXPECT_EQ(validator.validate("squeeze", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("squeeze", options_excessive), ValidationResult::INVALID);
}

TEST(Validator, SortCommandValid) {
    Validator validator;

    nlohmann::json sort_valid = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [],
            "options": {}
        }
    )");

    nlohmann::json sort_valid_name = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [ "name" ],
            "options": {}
        }
    )");

    nlohmann::json sort_valid_ext = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [ "ext" ],
            "options": {}
        }
    )");

    nlohmann::json sort_valid_date = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [ "date" ],
            "options": {}
        }
    )");

    nlohmann::json sort_valid_size = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [ "size" ],
            "options": {}
        }
    )");

    nlohmann::json sort_valid_inv = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [ "inv" ],
            "options": {}
        }
    )");

    nlohmann::json sort_valid_name_inv = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [ "name", "inv" ],
            "options": {}
        }
    )");

    nlohmann::json sort_valid_ext_inv = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [ "ext", "inv" ],
            "options": {}
        }
    )");

    nlohmann::json sort_valid_date_inv = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [ "date", "inv" ],
            "options": {}
        }
    )");

    nlohmann::json sort_valid_size_inv = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [ "size", "inv" ],
            "options": {}
        }
    )");

    EXPECT_EQ(validator.validate("sort", sort_valid), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("sort", sort_valid_name), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("sort", sort_valid_ext), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("sort", sort_valid_date), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("sort", sort_valid_size), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("sort", sort_valid_inv), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("sort", sort_valid_name_inv), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("sort", sort_valid_ext_inv), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("sort", sort_valid_date_inv), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("sort", sort_valid_size_inv), ValidationResult::VALID);
}

TEST(Validator, SortCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [ "name", "ext", "date" ],
            "options": {}
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [],
            "options": { "full": true }
        }
    )");

    nlohmann::json data_invalid = nlohmann::json::parse(R"(
        {
            "name": "sort",
            "data": [ "names" ],
            "options": {}
        }
    )");

    EXPECT_EQ(validator.validate("sort", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("sort", options_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("sort", data_invalid), ValidationResult::INVALID);
}

TEST(Validator, FreeCommandValid) {
    Validator validator;
    nlohmann::json free_valid = nlohmann::json::parse(R"(
        {
            "name": "free",
            "data": [],
            "options": {}
        }
    )");
    EXPECT_EQ(validator.validate("free", free_valid), ValidationResult::VALID);
}

TEST(Validator, FreeCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "free",
            "data": [ "name" ],
            "options": {}
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "free",
            "data": [],
            "options": { "full": true }
        }
    )");

    EXPECT_EQ(validator.validate("free", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("free", options_excessive), ValidationResult::INVALID);
}

TEST(Validator, VolCommandValid) {
    Validator validator;
    nlohmann::json vol_valid = nlohmann::json::parse(R"(
        {
            "name": "vol",
            "data": [ "vol1" ],
            "options": {}
        }
    )");

    nlohmann::json vol_valid_two = nlohmann::json::parse(R"(
        {
            "name": "vol",
            "data": [ "vol1", "vol2" ],
            "options": {}
        }
    )");

    EXPECT_EQ(validator.validate("vol", vol_valid), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("vol", vol_valid_two), ValidationResult::VALID);
}

TEST(Validator, VolCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "vol",
            "data": [ "vol1", "vol2", "vol3" ],
            "options": {}
        }
    )");

    nlohmann::json data_insufficient = nlohmann::json::parse(R"(
        {
            "name": "vol",
            "data": [],
            "options": {}
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "vol",
            "data": [ "vol1", "vol2" ],
            "options": { "full": true }
        }
    )");

    nlohmann::json data_invalid_type = nlohmann::json::parse(R"(
        {
            "name": "vol",
            "data": [ 1 ],
            "options": {}
        }
    )");

    EXPECT_EQ(validator.validate("vol", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("vol", data_insufficient), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("vol", options_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("vol", data_invalid_type), ValidationResult::INVALID);
}

TEST(Validator, ExitCommandValid) {
    Validator validator;
    nlohmann::json exit_valid = nlohmann::json::parse(R"(
        {
            "name": "exit",
            "data": [],
            "options": {}
        }
    )");
    EXPECT_EQ(validator.validate("exit", exit_valid), ValidationResult::VALID);
}

TEST(Validator, ExitCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "exit",
            "data": [ "name" ],
            "options": {}
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "exit",
            "data": [],
            "options": { "full": true }
        }
    )");

    EXPECT_EQ(validator.validate("exit", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("exit", options_excessive), ValidationResult::INVALID);
}

TEST(Validator, HelpCommandValid) {
    Validator validator;
    nlohmann::json help_valid = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [],
            "options": {}
        }
    )");

    nlohmann::json help_valid_dir = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "dir" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_init = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "init" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_create = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "create" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_delete = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "delete" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_rename = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "rename" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_copy = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "copy" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_move = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "move" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_add = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "add" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_squeeze = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "squeeze" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_sort = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "sort" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_free = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "free" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_vol = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "vol" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_exit = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "exit" ],
            "options": {}
        }
    )");

    nlohmann::json help_valid_help = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "help" ],
            "options": {}
        }
    )");

    EXPECT_EQ(validator.validate("help", help_valid), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_dir), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_init), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_create), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_delete), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_rename), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_copy), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_move), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_add), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_squeeze), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_sort), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_free), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_vol), ValidationResult::VALID);
    EXPECT_EQ(validator.validate("help", help_valid_exit), ValidationResult::VALID);
}

TEST(Validator, HelpCommandInvalid) {
    Validator validator;
    nlohmann::json data_excessive = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [ "name", "name" ],
            "options": {}
        }
    )");

    nlohmann::json options_excessive = nlohmann::json::parse(R"(
        {
            "name": "help",
            "data": [],
            "options": { "full": true }
        }
    )");

    EXPECT_EQ(validator.validate("help", data_excessive), ValidationResult::INVALID);
    EXPECT_EQ(validator.validate("help", options_excessive), ValidationResult::INVALID);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
