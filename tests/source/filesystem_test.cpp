#include "filesystem/filesystem.hpp"

#include <gtest/gtest.h>

#include "commands/add_command.hpp"
#include "commands/copy_command.hpp"
#include "commands/create_command.hpp"
#include "commands/delete_command.hpp"
#include "commands/dir_command.hpp"
#include "commands/init_command.hpp"
#include "commands/move_command.hpp"
#include "commands/rename_command.hpp"
#include "commands/squeeze_command.hpp"
#include "commands/vol_command.hpp"
#include "filesystem/catalog.hpp"

using filesystem::Catalog;
using filesystem::Error;
using filesystem::FileSystem;

TEST(InitCommand, InitSuccessfully) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    FileSystem filesystem;
    InitCommand init_command(filesystem);
    EXPECT_EQ(init_command.execute(j), "OK");
    EXPECT_EQ(init_command.execute(j), ERROR + ": the file system has already been initialized");
}

TEST(InitCommand, InitSuccessfullyPointer) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    FileSystem filesystem, second;
    InitCommand* init_command = new InitCommand(filesystem);
    EXPECT_EQ(init_command->execute(j), "OK");
    EXPECT_EQ(init_command->execute(j), ERROR + ": the file system has already been initialized");
    AbstractCommand* a_init_command = new InitCommand(second);
    EXPECT_EQ(a_init_command->execute(j), "OK");
    EXPECT_EQ(a_init_command->execute(j), ERROR + ": the file system has already been initialized");
    EXPECT_EQ(filesystem.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL");
    EXPECT_EQ(second.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(second.get_info().get_volume_name(), "VOL");
}

TEST(CreateCommand, CreateFileSuccessfully) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 5}}}};
    nlohmann::json test4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 2}}}};
    FileSystem filesystem;
    InitCommand init_command(filesystem);
    init_command.execute(j);
    CreateCommand create_command(filesystem);
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test2), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test3), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test4), NO_FREE_SPACE);
}

TEST(CreateCommand, CreateFileError) {
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 1}}}};
    FileSystem filesystem;
    CreateCommand create_command(filesystem);
    EXPECT_EQ(create_command.execute(test1), ERROR + ": the file system has not been initialized");
}

TEST(CreateCommand, CreateFileSuccessfully2) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test5 = {{"name", "create"}, {"data", {"test5.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test6 = {{"name", "create"}, {"data", {"test6.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test7 = {{"name", "create"}, {"data", {"test7.txt"}}, {"options", {{"allocate", 1}}}};
    FileSystem filesystem;
    InitCommand init_command(filesystem);
    init_command.execute(j);
    CreateCommand create_command(filesystem);
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test2), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test3), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test4), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test5), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test6), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test7), NO_FREE_RECORDS);
    EXPECT_EQ(create_command.execute(test1), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(create_command.execute(test5), "FILE_ALREADY_EXISTS");
}

TEST(CreateAndDeleteCommand, CreateFileSuccessfully3) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 3}}}};
    nlohmann::json test4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test5 = {{"name", "create"}, {"data", {"test5.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test6 = {{"name", "create"}, {"data", {"test6.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test7 = {{"name", "create"}, {"data", {"test7.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test8 = {{"name", "create"}, {"data", {"test8.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json delete_test3 = {{"name", "delete"}, {"data", {"test3.txt"}}};
    nlohmann::json delete_test4 = {{"name", "delete"}, {"data", {"test4.txt"}}};
    FileSystem filesystem;
    InitCommand init_command(filesystem);
    init_command.execute(j);
    CreateCommand create_command(filesystem);
    DeleteCommand delete_command(filesystem);
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test2), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test3), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test4), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test5), OK + ": the file has been added");
    EXPECT_EQ(delete_command.execute(delete_test3), OK + ": the file has been removed");
    EXPECT_EQ(delete_command.execute(delete_test4), OK + ": the file has been removed");
    EXPECT_EQ(create_command.execute(test6), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test7), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test8), OK + ": the file has been added");
}

TEST(DeleteTestCommand, DeleteError) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    FileSystem filesystem;
    InitCommand init_command(filesystem);
    CreateCommand create_command(filesystem);
    DeleteCommand delete_command(filesystem);
    EXPECT_EQ(create_command.execute(test1), ERROR + ": the file system has not been initialized");
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test1.txt"}}};
    EXPECT_EQ(delete_command.execute(delete_test1), ERROR + ": the file system has not been initialized");
    init_command.execute(j);
    EXPECT_EQ(delete_command.execute(delete_test1), FILE_NOT_FOUND);
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(delete_command.execute(delete_test1), OK + ": the file has been removed");
}

TEST(RenameTestCommand, RenameFile) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test1.txt"}}};
    nlohmann::json delete_test2 = {{"name", "delete"}, {"data", {"test2.txt"}}};
    nlohmann::json rename_test1 = {{"name", "rename"}, {"data", {"test1.txt", "test2.txt"}}};
    FileSystem filesystem;
    InitCommand init_command(filesystem);
    CreateCommand create_command(filesystem);
    RenameCommand rename_command(filesystem);
    DeleteCommand delete_command(filesystem);
    EXPECT_EQ(create_command.execute(test1), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(rename_command.execute(rename_test1), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(j), "OK");
    EXPECT_EQ(rename_command.execute(rename_test1), FILE_NOT_FOUND);
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(rename_command.execute(rename_test1), OK + ": the file has been renamed");
    EXPECT_EQ(delete_command.execute(delete_test1), FILE_NOT_FOUND);
    EXPECT_EQ(delete_command.execute(delete_test2), OK + ": the file has been removed");
    EXPECT_EQ(rename_command.execute(rename_test1), FILE_NOT_FOUND);
}

TEST(RenameTestCommand, RenameToExistingFile) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 3}}}};
    nlohmann::json rename_test1 = {{"name", "rename"}, {"data", {"test1.txt", "test2.txt"}}};
    FileSystem filesystem;
    InitCommand init_command(filesystem);
    CreateCommand create_command(filesystem);
    RenameCommand rename_command(filesystem);
    EXPECT_EQ(init_command.execute(j), "OK");
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test2), OK + ": the file has been added");
    EXPECT_EQ(rename_command.execute(rename_test1), FILE_ALREADY_EXISTS);
}

TEST(CopyTestCommand, CopyTest) {
    FileSystem filesystem;
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json copy_test1 = {{"name", "copy"}, {"data", {"test3.txt", "test1.txt"}}};
    nlohmann::json copy_test2 = {{"name", "copy"}, {"data", {"test3.txt", "test5.txt"}}};
    nlohmann::json copy_test3 = {{"name", "copy"}, {"data", {"test1.txt", "test2.txt"}}};
    nlohmann::json copy_test4 = {{"name", "copy"}, {"data", {"test1.txt", "test3.txt"}}};
    nlohmann::json copy_test5 = {{"name", "copy"}, {"data", {"test1.txt", "test4.txt"}}};
    nlohmann::json copy_test6 = {{"name", "copy"}, {"data", {"test1.txt", "test5.txt"}}};
    nlohmann::json copy_test7 = {{"name", "copy"}, {"data", {"test1.txt", "test6.txt"}}};
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test3.txt"}}};
    InitCommand init_command(filesystem);
    CreateCommand create_command(filesystem);
    CopyCommand copy_command(filesystem);
    DeleteCommand delete_command(filesystem);
    EXPECT_EQ(create_command.execute(test1), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(copy_command.execute(copy_test1), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(j), "OK");
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test2), OK + ": the file has been added");
    EXPECT_EQ(copy_command.execute(copy_test1), FILE_ALREADY_EXISTS);
    EXPECT_EQ(copy_command.execute(copy_test2), FILE_NOT_FOUND);
    EXPECT_EQ(copy_command.execute(copy_test3), FILE_ALREADY_EXISTS);
    EXPECT_EQ(copy_command.execute(copy_test4), OK + ": the file has been added");
    EXPECT_EQ(copy_command.execute(copy_test5), OK + ": the file has been added");
    EXPECT_EQ(copy_command.execute(copy_test6), OK + ": the file has been added");
    EXPECT_EQ(copy_command.execute(copy_test7), NO_FREE_SPACE);
    EXPECT_EQ(delete_command.execute(delete_test1), OK + ": the file has been removed");
    EXPECT_EQ(copy_command.execute(copy_test7), OK + ": the file has been added");
}

TEST(MoveTestCommand, MoveTest) {
    FileSystem filesystem;
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json move_test1 = {{"name", "move"}, {"data", {"test3.txt", "test1.txt"}}};
    nlohmann::json move_test2 = {{"name", "move"}, {"data", {"test3.txt", "test5.txt"}}};
    nlohmann::json move_test3 = {{"name", "move"}, {"data", {"test1.txt", "test2.txt"}}};
    nlohmann::json move_test4 = {{"name", "move"}, {"data", {"test1.txt", "test3.txt"}}};
    nlohmann::json move_test5 = {{"name", "move"}, {"data", {"test1.txt", "test4.txt"}}};
    nlohmann::json move_test6 = {{"name", "move"}, {"data", {"test3.txt", "test1.txt"}}};
    nlohmann::json move_test7 = {{"name", "move"}, {"data", {"test1.txt", "test3.txt"}}};
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test1.txt"}}};
    nlohmann::json delete_test2 = {{"name", "delete"}, {"data", {"test3.txt"}}};
    InitCommand init_command(filesystem);
    CreateCommand create_command(filesystem);
    MoveCommand move_command(filesystem);
    DeleteCommand delete_command(filesystem);
    EXPECT_EQ(create_command.execute(test1), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(move_command.execute(move_test1), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(j), "OK");
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test2), OK + ": the file has been added");
    EXPECT_EQ(move_command.execute(move_test1), FILE_ALREADY_EXISTS);
    EXPECT_EQ(move_command.execute(move_test2), FILE_NOT_FOUND);
    EXPECT_EQ(move_command.execute(move_test3), FILE_ALREADY_EXISTS);
    EXPECT_EQ(move_command.execute(move_test4), OK + " the file has been moved");
    EXPECT_EQ(move_command.execute(move_test5), FILE_NOT_FOUND);
    EXPECT_EQ(move_command.execute(move_test6), OK + " the file has been moved");
    EXPECT_EQ(move_command.execute(move_test7), OK + " the file has been moved");
    EXPECT_EQ(delete_command.execute(delete_test1), FILE_NOT_FOUND);
    EXPECT_EQ(delete_command.execute(delete_test2), OK + ": the file has been removed");
}

TEST(VolTestCommand, VolTest) {
    FileSystem filesystem;
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json vol_test1{{"name", "vol"}, {"data", {"VOL_1"}}};
    nlohmann::json vol_test2{{"name", "vol"}, {"data", {"VOL_3", "User"}}};
    InitCommand init_command(filesystem);
    VolCommand vol_command(filesystem);
    EXPECT_EQ(vol_command.execute(vol_test1), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(j), "OK");
    EXPECT_EQ(filesystem.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL");
    EXPECT_EQ(vol_command.execute(vol_test1), OK + ": the volume ID has been changed");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL_1");
    EXPECT_EQ(vol_command.execute(vol_test2), OK + ": the volume ID has been changed");
    EXPECT_EQ(filesystem.get_info().get_owner_name(), "User");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL_3");
}

TEST(AddTestCommand, AddTest) {
    FileSystem filesystem;
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 3}}}};
    nlohmann::json test4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test5 = {{"name", "create"}, {"data", {"test5.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json add_test1 = {{"name", "add"}, {"data", {"test123.txt"}}, {"options", {{"size", 1}}}};
    nlohmann::json add_test2 = {{"name", "add"}, {"data", {"test1.txt"}}, {"options", {{"size", 10}}}};
    nlohmann::json add_test3 = {{"name", "add"}, {"data", {"test1.txt"}}, {"options", {{"size", 1}}}};
    nlohmann::json add_test4 = {{"name", "add"}, {"data", {"test2.txt"}}, {"options", {{"size", 1}}}};
    nlohmann::json add_test5 = {{"name", "add"}, {"data", {"test3.txt"}}, {"options", {{"size", 1}}}};
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test2.txt"}}};
    nlohmann::json delete_test2 = {{"name", "delete"}, {"data", {"test3.txt"}}};
    nlohmann::json add_test6 = {{"name", "add"}, {"data", {"test5.txt"}}, {"options", {{"size", 4}}}};
    nlohmann::json add_test7 = {{"name", "add"}, {"data", {"test5.txt"}}, {"options", {{"size", 3}}}};
    InitCommand init_command(filesystem);
    CreateCommand create_command(filesystem);
    AddCommand add_command(filesystem);
    DeleteCommand delete_command(filesystem);
    EXPECT_EQ(add_command.execute(add_test1), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(j), "OK");
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test2), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test3), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test4), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test5), OK + ": the file has been added");
    EXPECT_EQ(add_command.execute(add_test1), FILE_NOT_FOUND);
    EXPECT_EQ(add_command.execute(add_test2), NO_FREE_SPACE);
    EXPECT_EQ(add_command.execute(add_test3), OK + ": the file size has been increased");
    EXPECT_EQ(add_command.execute(add_test3), NO_FREE_SPACE);
    EXPECT_EQ(add_command.execute(add_test4), NO_FREE_SPACE);
    EXPECT_EQ(add_command.execute(add_test5), NO_FREE_SPACE);
    EXPECT_EQ(delete_command.execute(delete_test1), OK + ": the file has been removed");
    EXPECT_EQ(delete_command.execute(delete_test2), OK + ": the file has been removed");
    EXPECT_EQ(add_command.execute(add_test5), FILE_NOT_FOUND);
    EXPECT_EQ(add_command.execute(add_test3), OK + ": the file size has been increased");
    EXPECT_EQ(add_command.execute(add_test6), NO_FREE_SPACE);
    EXPECT_EQ(add_command.execute(add_test7), OK + ": the file size has been increased");
}

TEST(SquezeeTestCommand, SqueezeTest1) {
    FileSystem filesystem;
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 5}}}};
    nlohmann::json test4 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 3}}}};
    nlohmann::json test5 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test2.txt"}}};
    nlohmann::json empty;
    InitCommand init_command(filesystem);
    CreateCommand create_command(filesystem);
    DeleteCommand delete_command(filesystem);
    SqueezeCommand squeeze_command(filesystem);
    EXPECT_EQ(squeeze_command.execute(empty), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(j), "OK");
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test2), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test3), OK + ": the file has been added");
    EXPECT_EQ(delete_command.execute(delete_test1), OK + ": the file has been removed");
    auto& catalog = filesystem.get_catalog();
    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);
    EXPECT_EQ(create_command.execute(test4), NO_FREE_SPACE);
    EXPECT_EQ(squeeze_command.execute(empty), OK + ": fragmentation was completed successfully");
    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 1);
    EXPECT_EQ(create_command.execute(test4), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test5), NO_FREE_SPACE);
}

TEST(SquezeeTestCommand, SqueezeTest2) {
    FileSystem filesystem;
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 3}, {"rec", 20}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test5 = {{"name", "create"}, {"data", {"test5.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test6 = {{"name", "create"}, {"data", {"test6.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json empty;
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test2.txt"}}};
    nlohmann::json delete_test2 = {{"name", "delete"}, {"data", {"test5.txt"}}};
    InitCommand init_command(filesystem);
    CreateCommand create_command(filesystem);
    DeleteCommand delete_command(filesystem);
    SqueezeCommand squeeze_command(filesystem);
    EXPECT_EQ(squeeze_command.execute(empty), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(j), "OK");
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test2), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test3), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test4), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test5), OK + ": the file has been added");
    EXPECT_EQ(create_command.execute(test6), OK + ": the file has been added");
    EXPECT_EQ(squeeze_command.execute(empty), OK + ": fragmentation was completed successfully");
    auto& catalog = filesystem.get_catalog();
    EXPECT_EQ(catalog.get_busy_segments_count(), 2);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);
    EXPECT_EQ(delete_command.execute(delete_test1), OK + ": the file has been removed");
    EXPECT_EQ(delete_command.execute(delete_test2), OK + ": the file has been removed");
    EXPECT_EQ(catalog.get_busy_segments_count(), 2);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);
    EXPECT_EQ(squeeze_command.execute(empty), OK + ": fragmentation was completed successfully");
    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);
}

TEST(CatalogTest, RenameWithDifferentExtension) {
    FileSystem filesystem;
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 2}, {"rec", 10}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"data.bin"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json rename_test1 = {{"name", "rename"}, {"data", {"data.bin", "data.txt"}}};
    nlohmann::json dir_test = {{"name", "dir"}, {"data", nullptr}};
    InitCommand init_command(filesystem);
    CreateCommand create_command(filesystem);
    RenameCommand rename_command(filesystem);
    DirCommand dir_command(filesystem);
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();
    EXPECT_EQ(dir_command.execute(dir_test), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(j), "OK");
    EXPECT_EQ(create_command.execute(test1), OK + ": the file has been added");
    EXPECT_EQ(rename_command.execute(rename_test1), OK + ": the file has been renamed");
    auto res = dir_command.execute(dir_test);

    std::string expected_line = "data.txt 2 Blocks " + today;

    EXPECT_NE(res.find(expected_line), std::string::npos)
        << "Expected line '" << expected_line << "' not found in result:\n"
        << res;
}

/*TEST(CatalogTest, RenameToSameName) {
    Catalog catalog(3, 2, 10);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);

    EXPECT_EQ(catalog.rename("test1.txt", "test1.txt"), Error::NO_ERROR);

    std::vector<std::string> result = catalog.dir();
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "test1.txt 2 Blocks " + today);
}

TEST(CatalogTest, DirTest) {
    Catalog catalog(3, 2, 10);
    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test2.txt", 1), Error::NO_ERROR);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    std::vector<std::string> expected = {"test1.txt 2 Blocks " + today, "test2.txt 1 Blocks " + today};

    auto result = catalog.dir();

    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        size_t pos = result[i].find("Blocks");
        EXPECT_EQ(pos, 12);
        pos = result[i].find("test" + std::to_string(i + 1) + ".txt");
        EXPECT_EQ(pos, 0);
    }
}

TEST(CatalogTest, DirTest2) {
    Catalog catalog(3, 2, 10);

    EXPECT_EQ(catalog.create("data.bin", 5), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("temp.log", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("report.txt", 3), Error::NO_ERROR);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    std::vector<std::string> result = catalog.dir();
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "data.bin 5 Blocks " + today);
    EXPECT_EQ(result[1], "temp.log 2 Blocks " + today);
    EXPECT_EQ(result[2], "report.txt 3 Blocks " + today);

    EXPECT_EQ(catalog.remove("report.txt"), Error::NO_ERROR);
    auto res = catalog.dir();
    EXPECT_EQ(res.size(), 2);
    EXPECT_EQ(res[0], "data.bin 5 Blocks " + today);
    EXPECT_EQ(res[1], "temp.log 2 Blocks " + today);
}

TEST(CatalogTest, DirEmptyCatalog) {
    Catalog catalog(3, 2, 10);

    std::vector<std::string> result = catalog.dir();
    EXPECT_TRUE(result.empty());
}

TEST(CatalogTest, DirSingleFile) {
    Catalog catalog(3, 2, 10);

    EXPECT_EQ(catalog.create("data.bin", 5), Error::NO_ERROR);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    std::vector<std::string> result = catalog.dir();
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "data.bin 5 Blocks " + today);
}

TEST(CatalogTest, SortTest) {
    Catalog catalog(3, 2, 10);

    EXPECT_EQ(catalog.create("data.bin", 5), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("temp.log", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("report.txt", 3), Error::NO_ERROR);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    std::vector<std::string> sorted_by_ext = catalog.sort(false, true, false, false, false);
    EXPECT_EQ(sorted_by_ext[0], "data.bin 5 Blocks " + today);
    EXPECT_EQ(sorted_by_ext[1], "temp.log 2 Blocks " + today);
    EXPECT_EQ(sorted_by_ext[2], "report.txt 3 Blocks " + today);

    std::vector<std::string> sorted_by_size_inv = catalog.sort(false, false, false, true, true);
    EXPECT_EQ(sorted_by_size_inv[0], "data.bin 5 Blocks " + today);
    EXPECT_EQ(sorted_by_size_inv[1], "report.txt 3 Blocks " + today);
    EXPECT_EQ(sorted_by_size_inv[2], "temp.log 2 Blocks " + today);
}

TEST(CatalogTest, SortByName) {
    Catalog catalog(3, 2, 10);

    EXPECT_EQ(catalog.create("data.bin", 5), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("temp.log", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("report.txt", 3), Error::NO_ERROR);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    std::vector<std::string> sorted = catalog.sort(true, false, false, false, false);
    EXPECT_EQ(sorted[0], "data.bin 5 Blocks " + today);
    EXPECT_EQ(sorted[1], "report.txt 3 Blocks " + today);
    EXPECT_EQ(sorted[2], "temp.log 2 Blocks " + today);
}

TEST(CatalogTest, SortByDate) {
    Catalog catalog(3, 2, 10);

    EXPECT_EQ(catalog.create("data.bin", 5), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("temp.log", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("report.txt", 3), Error::NO_ERROR);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    std::vector<std::string> sorted = catalog.sort(false, false, true, false, false);
    EXPECT_EQ(sorted[0], "data.bin 5 Blocks " + today);
    EXPECT_EQ(sorted[1], "temp.log 2 Blocks " + today);
    EXPECT_EQ(sorted[2], "report.txt 3 Blocks " + today);
}

TEST(CatalogTest, SortWithInverse) {
    Catalog catalog(3, 2, 10);

    EXPECT_EQ(catalog.create("data.bin", 5), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("temp.log", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("report.txt", 3), Error::NO_ERROR);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    std::vector<std::string> sorted = catalog.sort(true, false, false, false, true);
    EXPECT_EQ(sorted[0], "temp.log 2 Blocks " + today);
    EXPECT_EQ(sorted[1], "report.txt 3 Blocks " + today);
    EXPECT_EQ(sorted[2], "data.bin 5 Blocks " + today);
}

TEST(CatalogTest, SortEmptyCatalog) {
    Catalog catalog(3, 2, 10);

    std::vector<std::string> sorted = catalog.sort(true, false, false, false, false);
    EXPECT_TRUE(sorted.empty());
}

TEST(CatalogTest, SortSingleFile) {
    Catalog catalog(3, 2, 10);

    EXPECT_EQ(catalog.create("data.bin", 5), Error::NO_ERROR);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    std::vector<std::string> sorted = catalog.sort(true, false, false, false, false);
    EXPECT_EQ(sorted.size(), 1);
    EXPECT_EQ(sorted[0], "data.bin 5 Blocks " + today);
}

TEST(FileSystem, DirTest) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    FileSystem filesystem;
    filesystem.init("VOL", "OWNER", 3, 2, 10);
    EXPECT_EQ(filesystem.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(filesystem.create("test2.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(filesystem.create("test3.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(filesystem.create("test4.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(filesystem.remove("test3.txt"), Error::NO_ERROR);
    EXPECT_EQ(filesystem.remove("test4.txt"), Error::NO_ERROR);
    auto res = filesystem.dir(true);
    std::vector<std::string> expected = {"Volume:VOL, Owner:OWNER", "Free blocks:7", "Bad blocks:0",
                                         "test1.txt 2 Blocks " + today, "test2.txt 1 Blocks " + today};
    ASSERT_EQ(res.size(), expected.size());
    for (size_t i = 0; i < res.size(); ++i) {
        size_t pos;
        if (i == 0) {
            pos = res[i].find("VOL");
            EXPECT_EQ(pos, 7);
            pos = res[i].find("OWNER");
            EXPECT_EQ(pos, 18);
            continue;
        }
        if (i == 1) {
            pos = res[i].find("7");
            EXPECT_EQ(pos, 12);
            continue;
        }
        if (i == 2) {
            pos = res[i].find("0");
            EXPECT_EQ(pos, 11);
            continue;
        }
        pos = res[i].find("Blocks");
        EXPECT_EQ(pos, 12);
    }
}*/

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
