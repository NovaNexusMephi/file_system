#include "command_builder/command_builder.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <iomanip>
#include <memory>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

#include "commands/abstract_command.hpp"
#include "commands/add_command.hpp"
#include "commands/commands_constants.hpp"
#include "commands/copy_command.hpp"
#include "commands/create_command.hpp"
#include "commands/delete_command.hpp"
#include "commands/dir_command.hpp"
#include "commands/init_command.hpp"
#include "commands/invalid_command.hpp"
#include "commands/move_command.hpp"
#include "commands/rename_command.hpp"
#include "commands/squeeze_command.hpp"
#include "commands/vol_command.hpp"
#include "filesystem/catalog.hpp"
#include "filesystem/filesystem.hpp"

using command_builder::CommandBuilder;
using filesystem::Catalog;
using filesystem::Error;
using filesystem::FileSystem;
using nlohmann::json;

TEST(CommandBuilderInit, InitSuccessfully) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    json second_init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    auto second_init_command = builder.build(second_init_json);
    ASSERT_NE(second_init_command, nullptr);
    EXPECT_EQ(second_init_command->execute(), "ERROR: the file system has already been initialized");
}

TEST(CommandBuilderInit, InitSuccessfullyPointer) {
    FileSystem filesystem, second;
    CommandBuilder builder_fs1(filesystem);
    CommandBuilder builder_fs2(second);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};

    auto init_command_ptr = builder_fs1.build(init_json);
    ASSERT_NE(init_command_ptr, nullptr);
    EXPECT_EQ(init_command_ptr->execute(), "OK");
    auto second_init_command_ptr = builder_fs1.build(init_json);
    ASSERT_NE(second_init_command_ptr, nullptr);
    EXPECT_EQ(second_init_command_ptr->execute(), "ERROR: the file system has already been initialized");

    auto a_init_command_ptr = builder_fs2.build(init_json);
    ASSERT_NE(a_init_command_ptr, nullptr);
    EXPECT_EQ(a_init_command_ptr->execute(), "OK");
    auto second_a_init_command_ptr = builder_fs2.build(init_json);
    ASSERT_NE(second_a_init_command_ptr, nullptr);
    EXPECT_EQ(second_a_init_command_ptr->execute(), "ERROR: the file system has already been initialized");

    EXPECT_EQ(filesystem.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL");
    EXPECT_EQ(second.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(second.get_info().get_volume_name(), "VOL");
}

TEST(CommandBuilderCreate, CreateFileSuccessfully) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    init_command->execute();

    json create_json1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    auto create_test1 = builder.build(create_json1);
    ASSERT_NE(create_test1, nullptr);

    json create_json2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 2}}}};
    auto create_test2 = builder.build(create_json2);
    ASSERT_NE(create_test2, nullptr);

    json create_json3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 5}}}};
    auto create_test3 = builder.build(create_json3);
    ASSERT_NE(create_test3, nullptr);

    json create_json4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 2}}}};
    auto create_test4 = builder.build(create_json4);
    ASSERT_NE(create_test4, nullptr);

    EXPECT_EQ(create_test1->execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(create_test4->execute(), "NO_FREE_SPACE");
}

TEST(CommandBuilderCreate, CreateFileError) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json create_json = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 1}}}};
    auto create_test1 = builder.build(create_json);
    ASSERT_NE(create_test1, nullptr);
    EXPECT_EQ(create_test1->execute(), "ERROR: the file system has not been initialized");
}

TEST(CommandBuilderCreate, CreateFileSuccessfully2) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);
    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    init_command->execute();

    json create_json1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json5 = {{"name", "create"}, {"data", {"test5.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json6 = {{"name", "create"}, {"data", {"test6.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json7 = {{"name", "create"}, {"data", {"test7.txt"}}, {"options", {{"allocate", 1}}}};

    auto create_test1 = builder.build(create_json1);
    auto create_test2 = builder.build(create_json2);
    auto create_test3 = builder.build(create_json3);
    auto create_test4 = builder.build(create_json4);
    auto create_test5 = builder.build(create_json5);
    auto create_test6 = builder.build(create_json6);
    auto create_test7 = builder.build(create_json7);

    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    ASSERT_NE(create_test4, nullptr);
    ASSERT_NE(create_test5, nullptr);
    ASSERT_NE(create_test6, nullptr);
    ASSERT_NE(create_test7, nullptr);

    EXPECT_EQ(create_test1->execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(create_test4->execute(), "OK: the test4.txt has been added");
    EXPECT_EQ(create_test5->execute(), "OK: the test5.txt has been added");
    EXPECT_EQ(create_test6->execute(), "OK: the test6.txt has been added");
    EXPECT_EQ(create_test7->execute(), "NO_FREE_RECORDS");

    json create_json1_again = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json5_again = {{"name", "create"}, {"data", {"test5.txt"}}, {"options", {{"allocate", 1}}}};
    auto create_test1_again = builder.build(create_json1_again);
    auto create_test5_again = builder.build(create_json5_again);
    ASSERT_NE(create_test1_again, nullptr);
    ASSERT_NE(create_test5_again, nullptr);
    EXPECT_EQ(create_test1_again->execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(create_test5_again->execute(), "FILE_ALREADY_EXISTS");
}

TEST(CommandBuilderCreateDelete, CreateFileSuccessfully3) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);
    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    init_command->execute();

    json create_json1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    json create_json2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 3}}}};
    json create_json4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json5 = {{"name", "create"}, {"data", {"test5.txt"}}, {"options", {{"allocate", 2}}}};

    json delete_json3 = {{"name", "delete"}, {"data", {"test3.txt"}}};
    json delete_json4 = {{"name", "delete"}, {"data", {"test4.txt"}}};

    auto create_test1 = builder.build(create_json1);
    auto create_test2 = builder.build(create_json2);
    auto create_test3 = builder.build(create_json3);
    auto create_test4 = builder.build(create_json4);
    auto create_test5 = builder.build(create_json5);

    auto delete_test3 = builder.build(delete_json3);
    auto delete_test4 = builder.build(delete_json4);

    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    ASSERT_NE(create_test4, nullptr);
    ASSERT_NE(create_test5, nullptr);
    ASSERT_NE(delete_test3, nullptr);
    ASSERT_NE(delete_test4, nullptr);

    EXPECT_EQ(create_test1->execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(create_test4->execute(), "OK: the test4.txt has been added");
    EXPECT_EQ(create_test5->execute(), "OK: the test5.txt has been added");

    EXPECT_EQ(delete_test3->execute(), "OK: the test3.txt has been removed");
    EXPECT_EQ(delete_test4->execute(), "OK: the test4.txt has been removed");

    json create_json6_later = {{"name", "create"}, {"data", {"test6.txt"}}, {"options", {{"allocate", 2}}}};
    json create_json7_later = {{"name", "create"}, {"data", {"test7.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json8_later = {{"name", "create"}, {"data", {"test8.txt"}}, {"options", {{"allocate", 1}}}};
    auto create_test6_later = builder.build(create_json6_later);
    auto create_test7_later = builder.build(create_json7_later);
    auto create_test8_later = builder.build(create_json8_later);
    ASSERT_NE(create_test6_later, nullptr);
    ASSERT_NE(create_test7_later, nullptr);
    ASSERT_NE(create_test8_later, nullptr);

    EXPECT_EQ(create_test6_later->execute(), "OK: the test6.txt has been added");
    EXPECT_EQ(create_test7_later->execute(), "OK: the test7.txt has been added");
    EXPECT_EQ(create_test8_later->execute(), "OK: the test8.txt has been added");
}

TEST(CommandBuilderDelete, DeleteError) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json create_json = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    json delete_json = {{"name", "delete"}, {"data", {"test1.txt"}}};
    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};

    auto create_test1 = builder.build(create_json);
    auto delete_test1 = builder.build(delete_json);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(delete_test1, nullptr);

    EXPECT_EQ(create_test1->execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(delete_test1->execute(), "ERROR: the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    init_command->execute();

    auto delete_test1_after_init = builder.build(delete_json);
    ASSERT_NE(delete_test1_after_init, nullptr);
    EXPECT_EQ(delete_test1_after_init->execute(), "FILE_NOT_FOUND");

    auto create_test1_after_init = builder.build(create_json);
    ASSERT_NE(create_test1_after_init, nullptr);
    EXPECT_EQ(create_test1_after_init->execute(), "OK: the test1.txt has been added");

    auto delete_test1_after_create = builder.build(delete_json);
    ASSERT_NE(delete_test1_after_create, nullptr);
    EXPECT_EQ(delete_test1_after_create->execute(), "OK: the test1.txt has been removed");
}

TEST(CommandBuilderRename, RenameFile) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json create_json = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    json delete1_json = {{"name", "delete"}, {"data", {"test1.txt"}}};
    json delete2_json = {{"name", "delete"}, {"data", {"test2.txt"}}};
    json rename_json = {{"name", "rename"}, {"data", {"test1.txt", "test2.txt"}}};

    auto create_test1 = builder.build(create_json);
    auto rename_test1 = builder.build(rename_json);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(rename_test1, nullptr);

    EXPECT_EQ(create_test1->execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(rename_test1->execute(), "ERROR: the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto rename_test1_after_init = builder.build(rename_json);
    ASSERT_NE(rename_test1_after_init, nullptr);
    EXPECT_EQ(rename_test1_after_init->execute(), "FILE_NOT_FOUND");

    auto create_test1_after_init = builder.build(create_json);
    ASSERT_NE(create_test1_after_init, nullptr);
    EXPECT_EQ(create_test1_after_init->execute(), "OK: the test1.txt has been added");

    auto rename_test1_after_create = builder.build(rename_json);
    ASSERT_NE(rename_test1_after_create, nullptr);
    EXPECT_EQ(rename_test1_after_create->execute(), "OK: the test1.txt has been renamed");

    auto delete_test1_after_rename = builder.build(delete1_json);
    ASSERT_NE(delete_test1_after_rename, nullptr);
    EXPECT_EQ(delete_test1_after_rename->execute(), "FILE_NOT_FOUND");

    auto delete_test2_after_rename = builder.build(delete2_json);
    ASSERT_NE(delete_test2_after_rename, nullptr);
    EXPECT_EQ(delete_test2_after_rename->execute(), "OK: the test2.txt has been removed");

    auto rename_test1_after_delete = builder.build(rename_json);
    ASSERT_NE(rename_test1_after_delete, nullptr);
    EXPECT_EQ(rename_test1_after_delete->execute(), "FILE_NOT_FOUND");
}

TEST(CommandBuilderRename, RenameToExistingFile) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json create1_json = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    json create2_json = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 3}}}};
    json rename_json = {{"name", "rename"}, {"data", {"test1.txt", "test2.txt"}}};

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto create_test1 = builder.build(create1_json);
    ASSERT_NE(create_test1, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the test1.txt has been added");

    auto create_test2 = builder.build(create2_json);
    ASSERT_NE(create_test2, nullptr);
    EXPECT_EQ(create_test2->execute(), "OK: the test2.txt has been added");

    auto rename_test1 = builder.build(rename_json);
    ASSERT_NE(rename_test1, nullptr);
    EXPECT_EQ(rename_test1->execute(), "FILE_ALREADY_EXISTS");
}

TEST(CommandBuilderCopy, CopyTest) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json create1_json = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    json create2_json = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    json delete3_json = {{"name", "delete"}, {"data", {"test3.txt"}}};

    auto create_test1 = builder.build(create1_json);
    json copy_json1 = {{"name", "copy"}, {"data", {"test1.txt", "test3.txt"}}};
    auto copy_test1 = builder.build(copy_json1);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(copy_test1, nullptr);

    EXPECT_EQ(create_test1->execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(copy_test1->execute(), "ERROR: the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto create_test1_after_init = builder.build(create1_json);
    auto create_test2_after_init = builder.build(create2_json);
    ASSERT_NE(create_test1_after_init, nullptr);
    ASSERT_NE(create_test2_after_init, nullptr);
    EXPECT_EQ(create_test1_after_init->execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2_after_init->execute(), "OK: the test2.txt has been added");

    json copy_json1_after_create = {{"name", "copy"}, {"data", {"test1.txt", "test2.txt"}}};
    json copy_json2_after_create = {{"name", "copy"}, {"data", {"test5.txt", "test3.txt"}}};
    json copy_json3_after_create = {{"name", "copy"}, {"data", {"test1.txt", "test2.txt"}}};
    json copy_json4_after_create = {{"name", "copy"}, {"data", {"test1.txt", "test3.txt"}}};
    json copy_json5_after_create = {{"name", "copy"}, {"data", {"test1.txt", "test4.txt"}}};
    json copy_json6_after_create = {{"name", "copy"}, {"data", {"test1.txt", "test5.txt"}}};
    json copy_json7_after_create = {{"name", "copy"}, {"data", {"test1.txt", "test6.txt"}}};

    auto copy_test1_after_create = builder.build(copy_json1_after_create);
    auto copy_test2_after_create = builder.build(copy_json2_after_create);
    auto copy_test3_after_create = builder.build(copy_json3_after_create);
    auto copy_test4_after_create = builder.build(copy_json4_after_create);
    auto copy_test5_after_create = builder.build(copy_json5_after_create);
    auto copy_test6_after_create = builder.build(copy_json6_after_create);
    auto copy_test7_after_create = builder.build(copy_json7_after_create);

    ASSERT_NE(copy_test1_after_create, nullptr);
    ASSERT_NE(copy_test2_after_create, nullptr);
    ASSERT_NE(copy_test3_after_create, nullptr);
    ASSERT_NE(copy_test4_after_create, nullptr);
    ASSERT_NE(copy_test5_after_create, nullptr);
    ASSERT_NE(copy_test6_after_create, nullptr);
    ASSERT_NE(copy_test7_after_create, nullptr);

    EXPECT_EQ(copy_test1_after_create->execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(copy_test2_after_create->execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(copy_test3_after_create->execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(copy_test4_after_create->execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(copy_test5_after_create->execute(), "OK: the test4.txt has been added");
    EXPECT_EQ(copy_test6_after_create->execute(), "OK: the test5.txt has been added");
    EXPECT_EQ(copy_test7_after_create->execute(), "NO_FREE_SPACE");

    auto delete_test1 = builder.build(delete3_json);
    ASSERT_NE(delete_test1, nullptr);
    EXPECT_EQ(delete_test1->execute(), "OK: the test3.txt has been removed");

    json copy_json7_after_delete = {{"name", "copy"}, {"data", {"test1.txt", "test6.txt"}}};
    auto copy_test7_after_delete = builder.build(copy_json7_after_delete);
    ASSERT_NE(copy_test7_after_delete, nullptr);
    EXPECT_EQ(copy_test7_after_delete->execute(), "OK: the test6.txt has been added");
}

TEST(CommandBuilderMove, MoveTest) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json create1_json = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    json create2_json = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    json delete1_json = {{"name", "delete"}, {"data", {"test1.txt"}}};
    json delete3_json = {{"name", "delete"}, {"data", {"test3.txt"}}};

    auto create_test1 = builder.build(create1_json);
    json move_json1 = {{"name", "move"}, {"data", {"test1.txt", "test3.txt"}}};
    auto move_test1 = builder.build(move_json1);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(move_test1, nullptr);

    EXPECT_EQ(create_test1->execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(move_test1->execute(), "ERROR: the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto create_test1_after_init = builder.build(create1_json);
    auto create_test2_after_init = builder.build(create2_json);
    ASSERT_NE(create_test1_after_init, nullptr);
    ASSERT_NE(create_test2_after_init, nullptr);
    EXPECT_EQ(create_test1_after_init->execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2_after_init->execute(), "OK: the test2.txt has been added");

    json move_json1_after_create = {{"name", "move"}, {"data", {"test1.txt", "test2.txt"}}};
    json move_json2_after_create = {{"name", "move"}, {"data", {"test5.txt", "test3.txt"}}};
    json move_json3_after_create = {{"name", "move"}, {"data", {"test1.txt", "test2.txt"}}};
    json move_json4_after_create = {{"name", "move"}, {"data", {"test1.txt", "test3.txt"}}};
    json move_json5_after_create = {{"name", "move"}, {"data", {"test1.txt", "test4.txt"}}};
    json move_json6_after_create = {{"name", "move"}, {"data", {"test3.txt", "test1.txt"}}};
    json move_json7_after_create = {{"name", "move"}, {"data", {"test1.txt", "test3.txt"}}};

    auto move_test1_after_create = builder.build(move_json1_after_create);
    auto move_test2_after_create = builder.build(move_json2_after_create);
    auto move_test3_after_create = builder.build(move_json3_after_create);
    auto move_test4_after_create = builder.build(move_json4_after_create);
    auto move_test5_after_create = builder.build(move_json5_after_create);
    auto move_test6_after_create = builder.build(move_json6_after_create);
    auto move_test7_after_create = builder.build(move_json7_after_create);

    ASSERT_NE(move_test1_after_create, nullptr);
    ASSERT_NE(move_test2_after_create, nullptr);
    ASSERT_NE(move_test3_after_create, nullptr);
    ASSERT_NE(move_test4_after_create, nullptr);
    ASSERT_NE(move_test5_after_create, nullptr);
    ASSERT_NE(move_test6_after_create, nullptr);
    ASSERT_NE(move_test7_after_create, nullptr);

    EXPECT_EQ(move_test1_after_create->execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(move_test1_after_create->execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(move_test2_after_create->execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(move_test3_after_create->execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(move_test4_after_create->execute(), "OK: the test3.txt has been moved");
    EXPECT_EQ(move_test5_after_create->execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(move_test6_after_create->execute(), "OK: the test1.txt has been moved");
    EXPECT_EQ(move_test7_after_create->execute(), "OK: the test3.txt has been moved");

    auto delete_test1 = builder.build(delete1_json);
    auto delete_test2 = builder.build(delete3_json);
    ASSERT_NE(delete_test1, nullptr);
    ASSERT_NE(delete_test2, nullptr);

    EXPECT_EQ(delete_test1->execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(delete_test2->execute(), "OK: the test3.txt has been removed");
}

TEST(CommandBuilderVol, VolTest) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json vol1_json = {{"name", "vol"}, {"data", {"VOL_1"}}};
    json vol2_json = {{"name", "vol"}, {"data", {"VOL_3", "User"}}};

    auto vol_test1 = builder.build(vol1_json);
    ASSERT_NE(vol_test1, nullptr);
    EXPECT_EQ(vol_test1->execute(), "ERROR: the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    EXPECT_EQ(filesystem.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL");

    auto vol_test1_after_init = builder.build(vol1_json);
    ASSERT_NE(vol_test1_after_init, nullptr);
    EXPECT_EQ(vol_test1_after_init->execute(), "OK: the volume ID has been changed");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL_1");
    EXPECT_EQ(filesystem.get_info().get_owner_name(), "OWNER");

    auto vol_test2_after_init = builder.build(vol2_json);
    ASSERT_NE(vol_test2_after_init, nullptr);
    EXPECT_EQ(vol_test2_after_init->execute(), "OK: the volume ID has been changed");
    EXPECT_EQ(filesystem.get_info().get_owner_name(), "User");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL_3");
}

TEST(CommandBuilderAdd, AddTest) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};

    json add_json1_before_init = {{"name", "add"}, {"data", {"test123.txt"}}, {"options", {{"size", 1}}}};
    auto add_test1_before_init = builder.build(add_json1_before_init);
    ASSERT_NE(add_test1_before_init, nullptr);
    EXPECT_EQ(add_test1_before_init->execute(), "ERROR: the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    json create_json1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    json create_json2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 3}}}};
    json create_json4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 1}}}};
    json create_json5 = {{"name", "create"}, {"data", {"test5.txt"}}, {"options", {{"allocate", 2}}}};
    auto create_test1 = builder.build(create_json1);
    auto create_test2 = builder.build(create_json2);
    auto create_test3 = builder.build(create_json3);
    auto create_test4 = builder.build(create_json4);
    auto create_test5 = builder.build(create_json5);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    ASSERT_NE(create_test4, nullptr);
    ASSERT_NE(create_test5, nullptr);

    EXPECT_EQ(create_test1->execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(create_test4->execute(), "OK: the test4.txt has been added");
    EXPECT_EQ(create_test5->execute(), "OK: the test5.txt has been added");

    json add_json1 = {{"name", "add"}, {"data", {"test123.txt"}}, {"options", {{"size", 1}}}};
    json add_json2 = {{"name", "add"}, {"data", {"test1.txt"}}, {"options", {{"size", 10}}}};
    json add_json3 = {{"name", "add"}, {"data", {"test1.txt"}}, {"options", {{"size", 1}}}};
    json add_json3_again = {{"name", "add"}, {"data", {"test1.txt"}}, {"options", {{"size", 1}}}};
    json add_json4 = {{"name", "add"}, {"data", {"test2.txt"}}, {"options", {{"size", 1}}}};
    json add_json5 = {{"name", "add"}, {"data", {"test3.txt"}}, {"options", {{"size", 1}}}};
    auto add_test1 = builder.build(add_json1);
    auto add_test2 = builder.build(add_json2);
    auto add_test3 = builder.build(add_json3);
    auto add_test3_again = builder.build(add_json3_again);
    auto add_test4 = builder.build(add_json4);
    auto add_test5 = builder.build(add_json5);
    ASSERT_NE(add_test1, nullptr);
    ASSERT_NE(add_test2, nullptr);
    ASSERT_NE(add_test3, nullptr);
    ASSERT_NE(add_test3_again, nullptr);
    ASSERT_NE(add_test4, nullptr);
    ASSERT_NE(add_test5, nullptr);

    EXPECT_EQ(add_test1->execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(add_test2->execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test3->execute(), "OK: the test1.txt size has been increased");
    EXPECT_EQ(add_test3_again->execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test4->execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test5->execute(), "NO_FREE_SPACE");

    json delete_json1 = {{"name", "delete"}, {"data", {"test2.txt"}}};
    json delete_json2 = {{"name", "delete"}, {"data", {"test3.txt"}}};
    auto delete_test1 = builder.build(delete_json1);
    auto delete_test2 = builder.build(delete_json2);
    ASSERT_NE(delete_test1, nullptr);
    ASSERT_NE(delete_test2, nullptr);
    EXPECT_EQ(delete_test1->execute(), "OK: the test2.txt has been removed");
    EXPECT_EQ(delete_test2->execute(), "OK: the test3.txt has been removed");

    json add_json5_after_delete = {{"name", "add"}, {"data", {"test3.txt"}}, {"options", {{"size", 1}}}};
    json add_json3_after_delete = {{"name", "add"}, {"data", {"test1.txt"}}, {"options", {{"size", 1}}}};
    json add_json6_after_delete = {{"name", "add"}, {"data", {"test5.txt"}}, {"options", {{"size", 4}}}};
    json add_json7_after_delete = {{"name", "add"}, {"data", {"test5.txt"}}, {"options", {{"size", 3}}}};
    auto add_test5_after_delete = builder.build(add_json5_after_delete);
    auto add_test3_after_delete = builder.build(add_json3_after_delete);
    auto add_test6_after_delete = builder.build(add_json6_after_delete);
    auto add_test7_after_delete = builder.build(add_json7_after_delete);

    ASSERT_NE(add_test5_after_delete, nullptr);
    ASSERT_NE(add_test3_after_delete, nullptr);
    EXPECT_EQ(add_test5_after_delete->execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(add_test3_after_delete->execute(), "OK: the test1.txt size has been increased");
    EXPECT_EQ(add_test6_after_delete->execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test7_after_delete->execute(), "OK: the test5.txt size has been increased");
}

TEST(CommandBuilderSqueeze, SqueezeTest1) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json squeeze_json = {{"name", "squeeze"}, {"data", json::array()}};

    auto create_cmd = [&](const std::string& filename, int allocate) {
        json j = {{"name", "create"}, {"data", {filename}}, {"options", {{"allocate", allocate}}}};
        return builder.build(j);
    };
    auto delete_cmd = [&](const std::string& filename) {
        json j = {{"name", "delete"}, {"data", {filename}}};
        return builder.build(j);
    };

    auto squeeze_command_before_init = builder.build(squeeze_json);
    ASSERT_NE(squeeze_command_before_init, nullptr);
    EXPECT_EQ(squeeze_command_before_init->execute(), "ERROR: the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto create_test1 = create_cmd("test1.txt", 2);
    auto create_test2 = create_cmd("test2.txt", 2);
    auto create_test3 = create_cmd("test3.txt", 5);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the test3.txt has been added");

    auto delete_test1 = delete_cmd("test2.txt");
    ASSERT_NE(delete_test1, nullptr);
    EXPECT_EQ(delete_test1->execute(), "OK: the test2.txt has been removed");

    auto& catalog = filesystem.get_catalog();
    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);

    auto create_test4 = create_cmd("test2_new.txt", 3);
    ASSERT_NE(create_test4, nullptr);
    EXPECT_EQ(create_test4->execute(), "NO_FREE_SPACE");

    auto squeeze_command = builder.build(squeeze_json);
    ASSERT_NE(squeeze_command, nullptr);
    EXPECT_EQ(squeeze_command->execute(), "OK: fragmentation was completed successfully");

    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 1);

    auto create_test4_after_squeeze = create_cmd("test2_new.txt", 3);
    auto create_test5_after_squeeze = create_cmd("test4.txt", 2);
    ASSERT_NE(create_test4_after_squeeze, nullptr);
    ASSERT_NE(create_test5_after_squeeze, nullptr);
    EXPECT_EQ(create_test4_after_squeeze->execute(), "OK: the test2_new.txt has been added");  // Used 7+3=10
    EXPECT_EQ(create_test5_after_squeeze->execute(), "NO_FREE_SPACE");
}

TEST(CommandBuilderSqueeze, SqueezeTest2) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 3}, {"vol", 20}}}};
    json squeeze_json = {{"name", "squeeze"}, {"data", json::array()}};

    auto create_cmd = [&](const std::string& filename, int allocate) {
        json j = {{"name", "create"}, {"data", {filename}}, {"options", {{"allocate", allocate}}}};
        return builder.build(j);
    };
    auto delete_cmd = [&](const std::string& filename) {
        json j = {{"name", "delete"}, {"data", {filename}}};
        return builder.build(j);
    };

    auto squeeze_command_before_init = builder.build(squeeze_json);
    ASSERT_NE(squeeze_command_before_init, nullptr);
    EXPECT_EQ(squeeze_command_before_init->execute(), "ERROR: the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto create_test1 = create_cmd("test1.txt", 2);
    auto create_test2 = create_cmd("test2.txt", 2);
    auto create_test3 = create_cmd("test3.txt", 2);
    auto create_test4 = create_cmd("test4.txt", 2);
    auto create_test5 = create_cmd("test5.txt", 2);
    auto create_test6 = create_cmd("test6.txt", 2);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    ASSERT_NE(create_test4, nullptr);
    ASSERT_NE(create_test5, nullptr);
    ASSERT_NE(create_test6, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(create_test4->execute(), "OK: the test4.txt has been added");
    EXPECT_EQ(create_test5->execute(), "OK: the test5.txt has been added");
    EXPECT_EQ(create_test6->execute(), "OK: the test6.txt has been added");

    auto squeeze_command1 = builder.build(squeeze_json);
    ASSERT_NE(squeeze_command1, nullptr);
    EXPECT_EQ(squeeze_command1->execute(), "OK: fragmentation was completed successfully");

    auto& catalog = filesystem.get_catalog();
    EXPECT_EQ(catalog.get_busy_segments_count(), 2);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);

    auto delete_test1 = delete_cmd("test2.txt");
    auto delete_test2 = delete_cmd("test5.txt");
    ASSERT_NE(delete_test1, nullptr);
    ASSERT_NE(delete_test2, nullptr);
    EXPECT_EQ(delete_test1->execute(), "OK: the test2.txt has been removed");
    EXPECT_EQ(delete_test2->execute(), "OK: the test5.txt has been removed");

    EXPECT_EQ(catalog.get_busy_segments_count(), 2);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);

    auto squeeze_command2 = builder.build(squeeze_json);
    ASSERT_NE(squeeze_command2, nullptr);
    EXPECT_EQ(squeeze_command2->execute(), "OK: fragmentation was completed successfully");

    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);
}

TEST(CommandBuilderRename, RenameWithDifferentExtension) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json create_json = {{"name", "create"}, {"data", {"data.bin"}}, {"options", {{"allocate", 2}}}};
    json rename_json = {{"name", "rename"}, {"data", {"data.bin", "data.txt"}}};
    json dir_json = {{"name", "dir"}, {"data", json::array()}};

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    auto dir_command_before_init = builder.build(dir_json);
    ASSERT_NE(dir_command_before_init, nullptr);
    EXPECT_EQ(dir_command_before_init->execute(), "ERROR: the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto create_command = builder.build(create_json);
    ASSERT_NE(create_command, nullptr);
    EXPECT_EQ(create_command->execute(), "OK: the data.bin has been added");

    auto rename_command = builder.build(rename_json);
    ASSERT_NE(rename_command, nullptr);
    EXPECT_EQ(rename_command->execute(), "OK: the data.bin has been renamed");

    auto dir_command_after_rename = builder.build(dir_json);
    ASSERT_NE(dir_command_after_rename, nullptr);
    auto res = dir_command_after_rename->execute();
    std::string expected_line = "data.txt 2 Blocks " + today;

    EXPECT_NE(res.find(expected_line), std::string::npos)
        << "Expected line '" << expected_line << "' not found in result:\n"
        << res;
}

TEST(CommandBuilderDir, DirTest1) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json dir_json = {{"name", "dir"}, {"data", json::array()}};

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    auto dir_command_before_init = builder.build(dir_json);
    ASSERT_NE(dir_command_before_init, nullptr);
    EXPECT_EQ(dir_command_before_init->execute(), "ERROR: the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto dir_command_after_init = builder.build(dir_json);
    ASSERT_NE(dir_command_after_init, nullptr);
    EXPECT_EQ(dir_command_after_init->execute(), "OK:\n");

    json create_json1 = {{"name", "create"}, {"data", {"data.bin"}}, {"options", {{"allocate", 2}}}};
    json create_json2 = {{"name", "create"}, {"data", {"data.txt"}}, {"options", {{"allocate", 2}}}};
    json create_json3 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 3}}}};
    auto create_test1 = builder.build(create_json1);
    auto create_test2 = builder.build(create_json2);
    auto create_test3 = builder.build(create_json3);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the data.txt has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the test1.txt has been added");

    auto dir_command_after_create = builder.build(dir_json);
    ASSERT_NE(dir_command_after_create, nullptr);
    auto res = dir_command_after_create->execute();
    std::string expected_line = OK + ":\n" + "data.bin 2 Blocks " + today + "\n" + "data.txt 2 Blocks " + today + "\n" +
                                "test1.txt 3 Blocks " + today + "\n";
    EXPECT_EQ(res, expected_line);
}

TEST(CommandBuilderDir, DirTest2) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json dir_full_json = {{"name", "dir"}, {"data", {"full"}}};
    json delete_json = {{"name", "delete"}, {"data", {"test1.txt"}}};
    json vol1_json = {{"name", "vol"}, {"data", {"VOL_03"}}};
    json vol2_json = {{"name", "vol"}, {"data", {"VOL", "User"}}};

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    auto dir_command_before_init = builder.build(dir_full_json);
    ASSERT_NE(dir_command_before_init, nullptr);
    EXPECT_EQ(dir_command_before_init->execute(), "ERROR: the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto dir_cmd1 = builder.build(dir_full_json);
    ASSERT_NE(dir_cmd1, nullptr);
    auto res1 = dir_cmd1->execute();
    std::string expected1 = "OK:\nVolume:VOL, Owner:OWNER\nFree blocks:10\nBad blocks:0\n";
    EXPECT_EQ(res1, expected1);

    auto dir_cmd2 = builder.build(dir_full_json);
    ASSERT_NE(dir_cmd2, nullptr);
    EXPECT_EQ(dir_cmd2->execute(), expected1);

    json create_json1 = {{"name", "create"}, {"data", {"data.bin"}}, {"options", {{"allocate", 2}}}};
    json create_json2 = {{"name", "create"}, {"data", {"data.txt"}}, {"options", {{"allocate", 2}}}};
    json create_json3 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 3}}}};
    auto create_test1 = builder.build(create_json1);
    auto create_test2 = builder.build(create_json2);
    auto create_test3 = builder.build(create_json3);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the data.txt has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the test1.txt has been added");

    auto dir_cmd3 = builder.build(dir_full_json);
    ASSERT_NE(dir_cmd3, nullptr);
    auto res3 = dir_cmd3->execute();
    std::string expected3 = "OK:\nVolume:VOL, Owner:OWNER\nFree blocks:3\nBad blocks:0\ndata.bin 2 Blocks " + today +
                            "\n" + "data.txt 2 Blocks " + today + "\n" + "test1.txt 3 Blocks " + today + "\n";
    EXPECT_EQ(res3, expected3);

    auto delete_command = builder.build(delete_json);
    ASSERT_NE(delete_command, nullptr);
    EXPECT_EQ(delete_command->execute(), "OK: the test1.txt has been removed");

    auto dir_cmd4 = builder.build(dir_full_json);
    ASSERT_NE(dir_cmd4, nullptr);
    auto res4 = dir_cmd4->execute();
    std::string expected4 = "OK:\nVolume:VOL, Owner:OWNER\nFree blocks:6\nBad blocks:0\ndata.bin 2 Blocks " + today +
                            "\n" + "data.txt 2 Blocks " + today + "\n";
    EXPECT_EQ(res4, expected4);

    auto vol_command1 = builder.build(vol1_json);
    ASSERT_NE(vol_command1, nullptr);
    EXPECT_EQ(vol_command1->execute(), OK + ": the volume ID has been changed");

    auto dir_cmd5 = builder.build(dir_full_json);
    ASSERT_NE(dir_cmd5, nullptr);
    auto res5 = dir_cmd5->execute();
    std::string expected5 = "OK:\nVolume:VOL_03, Owner:OWNER\nFree blocks:6\nBad blocks:0\ndata.bin 2 Blocks " + today +
                            "\n" + "data.txt 2 Blocks " + today + "\n";
    EXPECT_EQ(res5, expected5);

    auto vol_command2 = builder.build(vol2_json);
    ASSERT_NE(vol_command2, nullptr);
    EXPECT_EQ(vol_command2->execute(), OK + ": the volume ID has been changed");

    auto dir_cmd6 = builder.build(dir_full_json);
    ASSERT_NE(dir_cmd6, nullptr);
    auto res6 = dir_cmd6->execute();
    std::string expected6 = "OK:\nVolume:VOL, Owner:User\nFree blocks:6\nBad blocks:0\ndata.bin 2 Blocks " + today +
                            "\n" + "data.txt 2 Blocks " + today + "\n";
    EXPECT_EQ(res6, expected6);
}

TEST(CommandBuilderSort, SortTest) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json sort_json = {{"name", "sort"}, {"data", {"ext"}}};

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    json create_json1 = {{"name", "create"}, {"data", {"data.bin"}}, {"options", {{"allocate", 5}}}};
    json create_json2 = {{"name", "create"}, {"data", {"temp.log"}}, {"options", {{"allocate", 2}}}};
    json create_json3 = {{"name", "create"}, {"data", {"report.txt"}}, {"options", {{"allocate", 3}}}};
    auto create_test1 = builder.build(create_json1);
    auto create_test2 = builder.build(create_json2);
    auto create_test3 = builder.build(create_json3);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the report.txt has been added");

    auto sort_command = builder.build(sort_json);
    ASSERT_NE(sort_command, nullptr);
    auto res = sort_command->execute();
    std::string expected =
        OK + ":\ndata.bin 5 Blocks " + today + "\ntemp.log 2 Blocks " + today + "\nreport.txt 3 Blocks " + today + "\n";
    EXPECT_EQ(res, expected);
}

TEST(CommandBuilderSort, SortTestByDate) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);
    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json sort_json = {{"name", "sort"}, {"data", {"date"}}};
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");
    json create_json1 = {{"name", "create"}, {"data", {"data.bin"}}, {"options", {{"allocate", 5}}}};
    json create_json2 = {{"name", "create"}, {"data", {"temp.log"}}, {"options", {{"allocate", 2}}}};
    json create_json3 = {{"name", "create"}, {"data", {"report.txt"}}, {"options", {{"allocate", 3}}}};
    auto create_test1 = builder.build(create_json1);
    auto create_test2 = builder.build(create_json2);
    auto create_test3 = builder.build(create_json3);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the report.txt has been added");

    auto sort_command = builder.build(sort_json);
    ASSERT_NE(sort_command, nullptr);
    auto res = sort_command->execute();
    std::string expected =
        OK + ":\ndata.bin 5 Blocks " + today + "\ntemp.log 2 Blocks " + today + "\nreport.txt 3 Blocks " + today + "\n";
    EXPECT_EQ(res, expected);
}

TEST(CommandBuilderSort, SortTestInverse) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);
    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json sort_name_json = {{"name", "sort"}, {"data", {"name"}}};
    json sort_name_inv_json = {{"name", "sort"}, {"data", {"name", "inv"}}};
    auto create_cmd = [&](const std::string& filename, int allocate) {
        json j = {{"name", "create"}, {"data", {filename}}, {"options", {{"allocate", allocate}}}};
        return builder.build(j);
    };
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");
    auto create_test1 = create_cmd("data.bin", 5);
    auto create_test2 = create_cmd("temp.log", 2);
    auto create_test3 = create_cmd("report.txt", 3);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the report.txt has been added");

    auto sort_command = builder.build(sort_name_json);
    ASSERT_NE(sort_command, nullptr);
    auto res = sort_command->execute();
    std::string expected =
        OK + ":\ndata.bin 5 Blocks " + today + "\nreport.txt 3 Blocks " + today + "\ntemp.log 2 Blocks " + today + "\n";
    EXPECT_EQ(res, expected);

    auto sort_command1 = builder.build(sort_name_inv_json);
    ASSERT_NE(sort_command1, nullptr);
    res = sort_command1->execute();
    expected =
        OK + ":\ntemp.log 2 Blocks " + today + "\nreport.txt 3 Blocks " + today + "\ndata.bin 5 Blocks " + today + "\n";
    EXPECT_EQ(res, expected);
}

TEST(CommandBuilderSort, SortTestSize) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);
    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json sort_json = {{"name", "sort"}, {"data", {"size"}}};
    auto create_cmd = [&](const std::string& filename, int allocate) {
        json j = {{"name", "create"}, {"data", {filename}}, {"options", {{"allocate", allocate}}}};
        return builder.build(j);
    };
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");
    auto create_test1 = create_cmd("data.bin", 5);
    auto create_test2 = create_cmd("temp.log", 2);
    auto create_test3 = create_cmd("report.txt", 3);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the report.txt has been added");

    auto sort_command = builder.build(sort_json);
    ASSERT_NE(sort_command, nullptr);
    auto res = sort_command->execute();
    std::string expected =
        OK + ":\ntemp.log 2 Blocks " + today + "\nreport.txt 3 Blocks " + today + "\ndata.bin 5 Blocks " + today + "\n";
    EXPECT_EQ(res, expected);
}

TEST(CommandBuilderSort, SortTestEmpty) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);
    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json sort_json = {{"name", "sort"}, {"data", {"size"}}};

    auto sort_command_before_init = builder.build(sort_json);
    ASSERT_NE(sort_command_before_init, nullptr);
    EXPECT_EQ(sort_command_before_init->execute(), ERROR + ": the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto sort_command_after_init = builder.build(sort_json);
    ASSERT_NE(sort_command_after_init, nullptr);
    auto res = sort_command_after_init->execute();
    EXPECT_EQ(res, "OK:\n");
}

TEST(CommandBuilderFree, FreeTest) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);
    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json free_json = {{"name", "free"}, {"data", json::array()}};
    auto create_cmd = [&](const std::string& filename, int allocate) {
        json j = {{"name", "create"}, {"data", {filename}}, {"options", {{"allocate", allocate}}}};
        return builder.build(j);
    };

    auto free_command_before_init = builder.build(free_json);
    ASSERT_NE(free_command_before_init, nullptr);
    EXPECT_EQ(free_command_before_init->execute(), ERROR + ": the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto create_test1 = create_cmd("data.bin", 5);
    auto create_test2 = create_cmd("temp.log", 2);
    auto create_test3 = create_cmd("report.txt", 3);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the report.txt has been added");

    auto free_command = builder.build(free_json);
    ASSERT_NE(free_command, nullptr);
    auto res = free_command->execute();
    std::string expected = "OK:\nOccupied: 10\nFree: 0\n";
    EXPECT_EQ(res, expected);
}

TEST(CommandBuilderFree, FreeTest2) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);
    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json free_json = {{"name", "free"}, {"data", json::array()}};
    json delete_json = {{"name", "delete"}, {"data", {"temp.log"}}};
    auto create_cmd = [&](const std::string& filename, int allocate) {
        json j = {{"name", "create"}, {"data", {filename}}, {"options", {{"allocate", allocate}}}};
        return builder.build(j);
    };

    auto free_command_before_init = builder.build(free_json);
    ASSERT_NE(free_command_before_init, nullptr);
    EXPECT_EQ(free_command_before_init->execute(), ERROR + ": the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto create_test1 = create_cmd("data.bin", 5);
    auto create_test2 = create_cmd("temp.log", 2);
    auto create_test3 = create_cmd("report.txt", 3);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the report.txt has been added");

    auto delete_test1 = builder.build(delete_json);
    ASSERT_NE(delete_test1, nullptr);
    EXPECT_EQ(delete_test1->execute(), "OK: the temp.log has been removed");

    auto free_command = builder.build(free_json);
    ASSERT_NE(free_command, nullptr);
    auto res = free_command->execute();
    std::string expected = "OK:\n1. 6 2\nOccupied: 8\nFree: 2\n";
    EXPECT_EQ(res, expected);
}

TEST(CommandBuilderFree, FreeTest3) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);
    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json free_json = {{"name", "free"}};
    json delete1_json = {{"name", "delete"}, {"data", {"data.bin"}}};
    json delete2_json = {{"name", "delete"}, {"data", {"report.txt"}}};
    auto create_cmd = [&](const std::string& filename, int allocate) {
        json j = {{"name", "create"}, {"data", {filename}}, {"options", {{"allocate", allocate}}}};
        return builder.build(j);
    };

    auto free_command_before_init = builder.build(free_json);
    ASSERT_NE(free_command_before_init, nullptr);
    EXPECT_EQ(free_command_before_init->execute(), ERROR + ": the file system has not been initialized");

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto create_test1 = create_cmd("data.bin", 5);
    auto create_test2 = create_cmd("temp.log", 2);
    auto create_test3 = create_cmd("report.txt", 3);
    ASSERT_NE(create_test1, nullptr);
    ASSERT_NE(create_test2, nullptr);
    ASSERT_NE(create_test3, nullptr);
    EXPECT_EQ(create_test1->execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2->execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3->execute(), "OK: the report.txt has been added");

    auto delete_test1 = builder.build(delete1_json);
    auto delete_test2 = builder.build(delete2_json);
    ASSERT_NE(delete_test1, nullptr);
    ASSERT_NE(delete_test2, nullptr);
    EXPECT_EQ(delete_test1->execute(), "OK: the data.bin has been removed");
    EXPECT_EQ(delete_test2->execute(), "OK: the report.txt has been removed");

    auto free_command = builder.build(free_json);
    ASSERT_NE(free_command, nullptr);
    auto res = free_command->execute();
    std::string expected = "OK:\n1. 1 5\n2. 8 3\nOccupied: 2\nFree: 8\n";
    EXPECT_EQ(res, expected);
}

TEST(CommandBuilderFree, FreeTest4) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);
    json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"rec", 2}, {"vol", 10}}}};
    json free_json = {{"name", "free"}, {"data", json::array()}};

    auto init_command = builder.build(init_json);
    ASSERT_NE(init_command, nullptr);
    EXPECT_EQ(init_command->execute(), "OK");

    auto free_command = builder.build(free_json);
    ASSERT_NE(free_command, nullptr);
    auto res = free_command->execute();
    std::string expected = "OK:\n1. 1 10\nOccupied: 0\nFree: 10\n";
    EXPECT_EQ(res, expected);
}

TEST(CommandBuilderHelp, HelpTest) {
    FileSystem filesystem;
    CommandBuilder builder(filesystem);

    json help_json_full = {{"name", "help"}, {"data", json::array()}};
    json help_json_init = {{"name", "help"}, {"data", {"init"}}};
    json help_json_create = {{"name", "help"}, {"data", {"create"}}};
    json help_json_delete = {{"name", "help"}, {"data", {"delete"}}};
    json help_json_rename = {{"name", "help"}, {"data", {"rename"}}};
    json help_json_copy = {{"name", "help"}, {"data", {"copy"}}};
    json help_json_move = {{"name", "help"}, {"data", {"move"}}};
    json help_json_add = {{"name", "help"}, {"data", {"add"}}};
    json help_json_squeeze = {{"name", "help"}, {"data", {"squeeze"}}};
    json help_json_sort = {{"name", "help"}, {"data", {"sort"}}};
    json help_json_free = {{"name", "help"}, {"data", {"free"}}};
    json help_json_vol = {{"name", "help"}, {"data", {"vol"}}};
    json help_json_exit = {{"name", "help"}, {"data", {"exit"}}};
    json help_json_help = {{"name", "help"}, {"data", {"help"}}};
    json help_json_dir = {{"name", "help"}, {"data", {"dir"}}};

    auto help_command_full = builder.build(help_json_full);
    auto help_command_init = builder.build(help_json_init);
    auto help_command_create = builder.build(help_json_create);
    auto help_command_delete = builder.build(help_json_delete);
    auto help_command_rename = builder.build(help_json_rename);
    auto help_command_copy = builder.build(help_json_copy);
    auto help_command_move = builder.build(help_json_move);
    auto help_command_add = builder.build(help_json_add);
    auto help_command_squeeze = builder.build(help_json_squeeze);
    auto help_command_sort = builder.build(help_json_sort);
    auto help_command_free = builder.build(help_json_free);
    auto help_command_vol = builder.build(help_json_vol);
    auto help_command_exit = builder.build(help_json_exit);
    auto help_command_help = builder.build(help_json_help);
    auto help_command_dir = builder.build(help_json_dir);

    ASSERT_NE(help_command_full, nullptr);
    ASSERT_NE(help_command_init, nullptr);
    ASSERT_NE(help_command_create, nullptr);
    ASSERT_NE(help_command_delete, nullptr);
    ASSERT_NE(help_command_rename, nullptr);
    ASSERT_NE(help_command_copy, nullptr);
    ASSERT_NE(help_command_move, nullptr);
    ASSERT_NE(help_command_add, nullptr);
    ASSERT_NE(help_command_squeeze, nullptr);
    ASSERT_NE(help_command_sort, nullptr);
    ASSERT_NE(help_command_free, nullptr);
    ASSERT_NE(help_command_vol, nullptr);
    ASSERT_NE(help_command_exit, nullptr);
    ASSERT_NE(help_command_help, nullptr);
    ASSERT_NE(help_command_dir, nullptr);

    EXPECT_EQ(help_command_full->execute(), OK + "\n" + HELP_FULL);
    EXPECT_EQ(help_command_init->execute(), OK + "\n" + HELP_INIT);
    EXPECT_EQ(help_command_create->execute(), OK + "\n" + HELP_CREATE);
    EXPECT_EQ(help_command_delete->execute(), OK + "\n" + HELP_DELETE);
    EXPECT_EQ(help_command_rename->execute(), OK + "\n" + HELP_RENAME);
    EXPECT_EQ(help_command_copy->execute(), OK + "\n" + HELP_COPY);
    EXPECT_EQ(help_command_move->execute(), OK + "\n" + HELP_MOVE);
    EXPECT_EQ(help_command_add->execute(), OK + "\n" + HELP_ADD);
    EXPECT_EQ(help_command_squeeze->execute(), OK + "\n" + HELP_SQUEEZE);
    EXPECT_EQ(help_command_sort->execute(), OK + "\n" + HELP_SORT);
    EXPECT_EQ(help_command_free->execute(), OK + "\n" + HELP_FREE);
    EXPECT_EQ(help_command_vol->execute(), OK + "\n" + HELP_VOL);
    EXPECT_EQ(help_command_exit->execute(), OK + "\n" + HELP_EXIT);
    EXPECT_EQ(help_command_help->execute(), OK + "\n" + HELP_HELP);
    EXPECT_EQ(help_command_dir->execute(), OK + "\n" + HELP_DIR);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
