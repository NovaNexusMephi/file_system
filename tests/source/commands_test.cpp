#include <gtest/gtest.h>

#include "commands/add_command.hpp"
#include "commands/commands_constants.hpp"
#include "commands/copy_command.hpp"
#include "commands/create_command.hpp"
#include "commands/delete_command.hpp"
#include "commands/dir_command.hpp"
#include "commands/free_command.hpp"
#include "commands/help_command.hpp"
#include "commands/init_command.hpp"
#include "commands/move_command.hpp"
#include "commands/rename_command.hpp"
#include "commands/sort_command.hpp"
#include "commands/squeeze_command.hpp"
#include "commands/vol_command.hpp"
#include "filesystem/catalog.hpp"
#include "filesystem/filesystem.hpp"

using filesystem::Catalog;
using filesystem::Error;
using filesystem::FileSystem;

TEST(InitCommand, InitSuccessfully) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    EXPECT_EQ(init_command.execute(), "OK");

    InitCommand second_init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    EXPECT_EQ(second_init_command.execute(), "ERROR: the file system has already been initialized");
}

TEST(InitCommand, InitSuccessfullyPointer) {
    FileSystem filesystem, second;

    InitCommand* init_command = new InitCommand(filesystem, "VOL", "OWNER", 3, 2, 10);
    EXPECT_EQ(init_command->execute(), "OK");
    EXPECT_EQ(init_command->execute(), "ERROR: the file system has already been initialized");

    AbstractCommand* a_init_command = new InitCommand(second, "VOL", "OWNER", 3, 2, 10);
    EXPECT_EQ(a_init_command->execute(), "OK");
    EXPECT_EQ(a_init_command->execute(), "ERROR: the file system has already been initialized");

    EXPECT_EQ(filesystem.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL");
    EXPECT_EQ(second.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(second.get_info().get_volume_name(), "VOL");

    delete init_command;
    delete a_init_command;
}

TEST(CreateCommand, CreateFileSuccessfully) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    init_command.execute();

    CreateCommand create_test1(filesystem, "test1.txt", 2);
    CreateCommand create_test2(filesystem, "test2.txt", 2);
    CreateCommand create_test3(filesystem, "test3.txt", 5);
    CreateCommand create_test4(filesystem, "test4.txt", 2);

    EXPECT_EQ(create_test1.execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(create_test4.execute(), "NO_FREE_SPACE");
}

TEST(CreateCommand, CreateFileError) {
    FileSystem filesystem;
    CreateCommand create_test1(filesystem, "test1.txt", 1);
    EXPECT_EQ(create_test1.execute(), "ERROR: the file system has not been initialized");
}

TEST(CreateCommand, CreateFileSuccessfully2) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    init_command.execute();

    CreateCommand create_test1(filesystem, "test1.txt", 1);
    CreateCommand create_test2(filesystem, "test2.txt", 1);
    CreateCommand create_test3(filesystem, "test3.txt", 1);
    CreateCommand create_test4(filesystem, "test4.txt", 1);
    CreateCommand create_test5(filesystem, "test5.txt", 1);
    CreateCommand create_test6(filesystem, "test6.txt", 1);
    CreateCommand create_test7(filesystem, "test7.txt", 1);

    EXPECT_EQ(create_test1.execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(create_test4.execute(), "OK: the test4.txt has been added");
    EXPECT_EQ(create_test5.execute(), "OK: the test5.txt has been added");
    EXPECT_EQ(create_test6.execute(), "OK: the test6.txt has been added");
    EXPECT_EQ(create_test7.execute(), "NO_FREE_RECORDS");

    EXPECT_EQ(create_test1.execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(create_test5.execute(), "FILE_ALREADY_EXISTS");
}

TEST(CreateAndDeleteCommand, CreateFileSuccessfully3) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    init_command.execute();

    CreateCommand create_test1(filesystem, "test1.txt", 2);
    CreateCommand create_test2(filesystem, "test2.txt", 1);
    CreateCommand create_test3(filesystem, "test3.txt", 3);
    CreateCommand create_test4(filesystem, "test4.txt", 1);
    CreateCommand create_test5(filesystem, "test5.txt", 2);
    CreateCommand create_test6(filesystem, "test6.txt", 2);
    CreateCommand create_test7(filesystem, "test7.txt", 1);
    CreateCommand create_test8(filesystem, "test8.txt", 1);

    DeleteCommand delete_test3(filesystem, "test3.txt");
    DeleteCommand delete_test4(filesystem, "test4.txt");

    EXPECT_EQ(create_test1.execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(create_test4.execute(), "OK: the test4.txt has been added");
    EXPECT_EQ(create_test5.execute(), "OK: the test5.txt has been added");

    EXPECT_EQ(delete_test3.execute(), "OK: the test3.txt has been removed");
    EXPECT_EQ(delete_test4.execute(), "OK: the test4.txt has been removed");

    EXPECT_EQ(create_test6.execute(), "OK: the test6.txt has been added");
    EXPECT_EQ(create_test7.execute(), "OK: the test7.txt has been added");
    EXPECT_EQ(create_test8.execute(), "OK: the test8.txt has been added");
}

TEST(DeleteTestCommand, DeleteError) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "test1.txt", 2);
    DeleteCommand delete_test1(filesystem, "test1.txt");

    EXPECT_EQ(create_test1.execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(delete_test1.execute(), "ERROR: the file system has not been initialized");

    init_command.execute();
    EXPECT_EQ(delete_test1.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(create_test1.execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(delete_test1.execute(), "OK: the test1.txt has been removed");
}

TEST(RenameTestCommand, RenameFile) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "test1.txt", 2);
    DeleteCommand delete_test1(filesystem, "test1.txt");
    DeleteCommand delete_test2(filesystem, "test2.txt");
    RenameCommand rename_test1(filesystem, "test1.txt", "test2.txt");

    EXPECT_EQ(create_test1.execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(rename_test1.execute(), "ERROR: the file system has not been initialized");

    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(rename_test1.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(create_test1.execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(rename_test1.execute(), "OK: the test1.txt has been renamed");
    EXPECT_EQ(delete_test1.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(delete_test2.execute(), "OK: the test2.txt has been removed");
    EXPECT_EQ(rename_test1.execute(), "FILE_NOT_FOUND");
}

TEST(RenameTestCommand, RenameToExistingFile) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "test1.txt", 2);
    CreateCommand create_test2(filesystem, "test2.txt", 3);
    RenameCommand rename_test1(filesystem, "test1.txt", "test2.txt");

    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(rename_test1.execute(), "FILE_ALREADY_EXISTS");
}

TEST(CopyTestCommand, CopyTest) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "test1.txt", 2);
    CreateCommand create_test2(filesystem, "test2.txt", 1);

    CopyCommand copy_test1(filesystem, "test3.txt", "test1.txt");
    CopyCommand copy_test2(filesystem, "test3.txt", "test5.txt");
    CopyCommand copy_test3(filesystem, "test1.txt", "test2.txt");
    CopyCommand copy_test4(filesystem, "test1.txt", "test3.txt");
    CopyCommand copy_test5(filesystem, "test1.txt", "test4.txt");
    CopyCommand copy_test6(filesystem, "test1.txt", "test5.txt");
    CopyCommand copy_test7(filesystem, "test1.txt", "test6.txt");

    DeleteCommand delete_test1(filesystem, "test3.txt");

    EXPECT_EQ(create_test1.execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(copy_test1.execute(), "ERROR: the file system has not been initialized");

    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the test2.txt has been added");

    EXPECT_EQ(copy_test1.execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(copy_test2.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(copy_test3.execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(copy_test4.execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(copy_test5.execute(), "OK: the test4.txt has been added");
    EXPECT_EQ(copy_test6.execute(), "OK: the test5.txt has been added");
    EXPECT_EQ(copy_test7.execute(), "NO_FREE_SPACE");

    EXPECT_EQ(delete_test1.execute(), "OK: the test3.txt has been removed");
    EXPECT_EQ(copy_test7.execute(), "OK: the test6.txt has been added");
}

TEST(MoveTestCommand, MoveTest) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "test1.txt", 2);
    CreateCommand create_test2(filesystem, "test2.txt", 1);

    MoveCommand move_test1(filesystem, "test3.txt", "test1.txt");
    MoveCommand move_test2(filesystem, "test3.txt", "test5.txt");
    MoveCommand move_test3(filesystem, "test1.txt", "test2.txt");
    MoveCommand move_test4(filesystem, "test1.txt", "test3.txt");
    MoveCommand move_test5(filesystem, "test1.txt", "test4.txt");
    MoveCommand move_test6(filesystem, "test3.txt", "test1.txt");
    MoveCommand move_test7(filesystem, "test1.txt", "test3.txt");

    DeleteCommand delete_test1(filesystem, "test1.txt");
    DeleteCommand delete_test2(filesystem, "test3.txt");

    EXPECT_EQ(create_test1.execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(move_test1.execute(), "ERROR: the file system has not been initialized");

    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the test2.txt has been added");

    EXPECT_EQ(move_test1.execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(move_test2.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(move_test3.execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(move_test4.execute(), "OK: the test3.txt has been moved");
    EXPECT_EQ(move_test5.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(move_test6.execute(), "OK: the test1.txt has been moved");
    EXPECT_EQ(move_test7.execute(), "OK: the test3.txt has been moved");

    EXPECT_EQ(delete_test1.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(delete_test2.execute(), "OK: the test3.txt has been removed");
}

TEST(VolTestCommand, VolTest) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    VolCommand vol_test1(filesystem, "VOL_1", "");
    VolCommand vol_test2(filesystem, "VOL_3", "User");

    EXPECT_EQ(vol_test1.execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(init_command.execute(), "OK");

    EXPECT_EQ(filesystem.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL");

    EXPECT_EQ(vol_test1.execute(), "OK: the volume ID has been changed");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL_1");

    EXPECT_EQ(vol_test2.execute(), "OK: the volume ID has been changed");
    EXPECT_EQ(filesystem.get_info().get_owner_name(), "User");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL_3");
}

TEST(AddTestCommand, AddTest) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);

    CreateCommand create_test1(filesystem, "test1.txt", 2);
    CreateCommand create_test2(filesystem, "test2.txt", 1);
    CreateCommand create_test3(filesystem, "test3.txt", 3);
    CreateCommand create_test4(filesystem, "test4.txt", 1);
    CreateCommand create_test5(filesystem, "test5.txt", 2);

    AddCommand add_test1(filesystem, "test123.txt", 1);
    AddCommand add_test2(filesystem, "test1.txt", 10);
    AddCommand add_test3(filesystem, "test1.txt", 1);
    AddCommand add_test4(filesystem, "test2.txt", 1);
    AddCommand add_test5(filesystem, "test3.txt", 1);
    AddCommand add_test6(filesystem, "test5.txt", 4);
    AddCommand add_test7(filesystem, "test5.txt", 3);

    DeleteCommand delete_test1(filesystem, "test2.txt");
    DeleteCommand delete_test2(filesystem, "test3.txt");

    EXPECT_EQ(add_test1.execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(init_command.execute(), "OK");

    EXPECT_EQ(create_test1.execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(create_test4.execute(), "OK: the test4.txt has been added");
    EXPECT_EQ(create_test5.execute(), "OK: the test5.txt has been added");

    EXPECT_EQ(add_test1.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(add_test2.execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test3.execute(), "OK: the test1.txt size has been increased");
    EXPECT_EQ(add_test3.execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test4.execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test5.execute(), "NO_FREE_SPACE");

    EXPECT_EQ(delete_test1.execute(), "OK: the test2.txt has been removed");
    EXPECT_EQ(delete_test2.execute(), "OK: the test3.txt has been removed");

    EXPECT_EQ(add_test5.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(add_test3.execute(), "OK: the test1.txt size has been increased");
    EXPECT_EQ(add_test6.execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test7.execute(), "OK: the test5.txt size has been increased");
}

TEST(SqueezeTestCommand, SqueezeTest1) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "test1.txt", 2);
    CreateCommand create_test2(filesystem, "test2.txt", 2);
    CreateCommand create_test3(filesystem, "test3.txt", 5);
    CreateCommand create_test4(filesystem, "test2.txt", 3);
    CreateCommand create_test5(filesystem, "test4.txt", 2);

    DeleteCommand delete_test1(filesystem, "test2.txt");
    SqueezeCommand squeeze_command(filesystem);

    EXPECT_EQ(squeeze_command.execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(init_command.execute(), "OK");

    EXPECT_EQ(create_test1.execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(delete_test1.execute(), "OK: the test2.txt has been removed");

    auto& catalog = filesystem.get_catalog();
    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);

    EXPECT_EQ(create_test4.execute(), "NO_FREE_SPACE");
    EXPECT_EQ(squeeze_command.execute(), "OK: fragmentation was completed successfully");

    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 1);

    EXPECT_EQ(create_test4.execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test5.execute(), "NO_FREE_SPACE");
}

TEST(SqueezeTestCommand, SqueezeTest2) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 3, 20);
    CreateCommand create_test1(filesystem, "test1.txt", 2);
    CreateCommand create_test2(filesystem, "test2.txt", 2);
    CreateCommand create_test3(filesystem, "test3.txt", 2);
    CreateCommand create_test4(filesystem, "test4.txt", 2);
    CreateCommand create_test5(filesystem, "test5.txt", 2);
    CreateCommand create_test6(filesystem, "test6.txt", 2);

    DeleteCommand delete_test1(filesystem, "test2.txt");
    DeleteCommand delete_test2(filesystem, "test5.txt");

    SqueezeCommand squeeze_command(filesystem);

    EXPECT_EQ(squeeze_command.execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(init_command.execute(), "OK");

    EXPECT_EQ(create_test1.execute(), "OK: the test1.txt has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the test2.txt has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the test3.txt has been added");
    EXPECT_EQ(create_test4.execute(), "OK: the test4.txt has been added");
    EXPECT_EQ(create_test5.execute(), "OK: the test5.txt has been added");
    EXPECT_EQ(create_test6.execute(), "OK: the test6.txt has been added");

    EXPECT_EQ(squeeze_command.execute(), "OK: fragmentation was completed successfully");

    auto& catalog = filesystem.get_catalog();
    EXPECT_EQ(catalog.get_busy_segments_count(), 2);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);

    EXPECT_EQ(delete_test1.execute(), "OK: the test2.txt has been removed");
    EXPECT_EQ(delete_test2.execute(), "OK: the test5.txt has been removed");

    EXPECT_EQ(catalog.get_busy_segments_count(), 2);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);

    EXPECT_EQ(squeeze_command.execute(), "OK: fragmentation was completed successfully");

    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);
}

TEST(RenameTestCommand, RenameWithDifferentExtension) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "data.bin", 2);
    RenameCommand rename_test1(filesystem, "data.bin", "data.txt");
    DirCommand dir_command(filesystem);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    EXPECT_EQ(dir_command.execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(init_command.execute(), "OK");

    EXPECT_EQ(create_test1.execute(), "OK: the data.bin has been added");
    EXPECT_EQ(rename_test1.execute(), "OK: the data.bin has been renamed");

    auto res = dir_command.execute();
    std::string expected_line = "data.txt 2 Blocks " + today;

    EXPECT_NE(res.find(expected_line), std::string::npos)
        << "Expected line '" << expected_line << "' not found in result:\n"
        << res;
}

TEST(DirTestCommand, DirTest1) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "data.bin", 2);
    CreateCommand create_test2(filesystem, "data.txt", 2);
    CreateCommand create_test3(filesystem, "test1.txt", 3);
    DirCommand dir_command(filesystem);
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    EXPECT_EQ(dir_command.execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(dir_command.execute(), "OK:\n");
    EXPECT_EQ(create_test1.execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the data.txt has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the test1.txt has been added");
    auto res = dir_command.execute();
    std::string expected_line = OK + ":\n" + "data.bin 2 Blocks " + today + "\n" + "data.txt 2 Blocks " + today + "\n" +
                                "test1.txt 3 Blocks " + today + "\n";
    auto value = res == expected_line;
    EXPECT_TRUE(value);
}

TEST(DirTestCommand, DirTest2) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "data.bin", 2);
    CreateCommand create_test2(filesystem, "data.txt", 2);
    CreateCommand create_test3(filesystem, "test1.txt", 3);
    DeleteCommand delete_command(filesystem, "test1.txt");
    VolCommand vol_command(filesystem, "VOL_03");
    VolCommand vol_command1(filesystem, "VOL", "User");
    DirCommand dir_command(filesystem, true);
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();
    EXPECT_EQ(dir_command.execute(), "ERROR: the file system has not been initialized");
    EXPECT_EQ(init_command.execute(), "OK");
    auto res = dir_command.execute();
    std::string expected = "OK:\nVolume:VOL, Owner:OWNER\nFree blocks:10\nBad blocks:0\n";
    auto value = res == expected;
    EXPECT_TRUE(value);
    EXPECT_EQ(dir_command.execute(), "OK:\nVolume:VOL, Owner:OWNER\nFree blocks:10\nBad blocks:0\n");
    EXPECT_EQ(create_test1.execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the data.txt has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the test1.txt has been added");
    res = dir_command.execute();
    std::string expected_line = expected + "data.bin 2 Blocks " + today + "\n" + "data.txt 2 Blocks " + today + "\n" +
                                "test1.txt 3 Blocks " + today + "\n";
    value = res == expected_line;
    EXPECT_FALSE(value);
    expected_line = "OK:\nVolume:VOL, Owner:OWNER\nFree blocks:3\nBad blocks:0\ndata.bin 2 Blocks " + today + "\n" +
                    "data.txt 2 Blocks " + today + "\n" + "test1.txt 3 Blocks " + today + "\n";
    value = res == expected_line;
    EXPECT_TRUE(value);
    EXPECT_EQ(delete_command.execute(), "OK: the test1.txt has been removed");
    res = dir_command.execute();
    expected_line = "OK:\nVolume:VOL, Owner:OWNER\nFree blocks:6\nBad blocks:0\ndata.bin 2 Blocks " + today + "\n" +
                    "data.txt 2 Blocks " + today + "\n";
    value = res == expected_line;
    EXPECT_TRUE(value);
    EXPECT_EQ(vol_command.execute(), OK + ": the volume ID has been changed");
    res = dir_command.execute();
    expected_line = "OK:\nVolume:VOL_03, Owner:OWNER\nFree blocks:6\nBad blocks:0\ndata.bin 2 Blocks " + today + "\n" +
                    "data.txt 2 Blocks " + today + "\n";
    value = res == expected_line;
    EXPECT_TRUE(value);
    EXPECT_EQ(vol_command1.execute(), OK + ": the volume ID has been changed");
    res = dir_command.execute();
    expected_line = "OK:\nVolume:VOL, Owner:User\nFree blocks:6\nBad blocks:0\ndata.bin 2 Blocks " + today + "\n" +
                    "data.txt 2 Blocks " + today + "\n";
    value = res == expected_line;
    EXPECT_TRUE(value);
}

TEST(SortCommandTest, SortTest) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "data.bin", 5);
    CreateCommand create_test2(filesystem, "temp.log", 2);
    CreateCommand create_test3(filesystem, "report.txt", 3);
    SortCommand sort_command(filesystem, "ext");
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();
    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the report.txt has been added");
    auto res = sort_command.execute();
    std::string expected =
        OK + ":\ndata.bin 5 Blocks " + today + "\ntemp.log 2 Blocks " + today + "\nreport.txt 3 Blocks " + today + "\n";
    EXPECT_TRUE(res == expected);
}

TEST(SortCommandTest, SortTestByDate) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "data.bin", 5);
    CreateCommand create_test2(filesystem, "temp.log", 2);
    CreateCommand create_test3(filesystem, "report.txt", 3);
    SortCommand sort_command(filesystem, "date");
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();
    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the report.txt has been added");
    auto res = sort_command.execute();
    std::string expected =
        OK + ":\ndata.bin 5 Blocks " + today + "\ntemp.log 2 Blocks " + today + "\nreport.txt 3 Blocks " + today + "\n";
    EXPECT_TRUE(res == expected);
}

TEST(SortCommandTest, SortTestInverse) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "data.bin", 5);
    CreateCommand create_test2(filesystem, "temp.log", 2);
    CreateCommand create_test3(filesystem, "report.txt", 3);
    SortCommand sort_command(filesystem, "name");
    SortCommand sort_command1(filesystem, "name", "inv");
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();
    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the report.txt has been added");
    auto res = sort_command.execute();
    std::string expected =
        OK + ":\ndata.bin 5 Blocks " + today + "\nreport.txt 3 Blocks " + today + "\ntemp.log 2 Blocks " + today + "\n";
    EXPECT_TRUE(res == expected);
    res = sort_command1.execute();
    expected =
        OK + ":\ntemp.log 2 Blocks " + today + "\nreport.txt 3 Blocks " + today + "\ndata.bin 5 Blocks " + today + "\n";
    EXPECT_TRUE(res == expected);
}

TEST(SortCommandTest, SortTestInverse1) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "data.bin", 5);
    CreateCommand create_test2(filesystem, "temp.log", 2);
    CreateCommand create_test3(filesystem, "report.txt", 3);
    SortCommand sort_command(filesystem, "", "inv");
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();
    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the report.txt has been added");
    auto res = sort_command.execute();
    std::string expected =
        OK + ":\ntemp.log 2 Blocks " + today + "\nreport.txt 3 Blocks " + today + "\ndata.bin 5 Blocks " + today + "\n";
    EXPECT_TRUE(res == expected);
}

TEST(SortCommandTest, SortTestSize) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "data.bin", 5);
    CreateCommand create_test2(filesystem, "temp.log", 2);
    CreateCommand create_test3(filesystem, "report.txt", 3);
    SortCommand sort_command(filesystem, "size");
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();
    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the report.txt has been added");
    auto res = sort_command.execute();
    std::string expected =
        OK + ":\ntemp.log 2 Blocks " + today + "\nreport.txt 3 Blocks " + today + "\ndata.bin 5 Blocks " + today + "\n";
    EXPECT_TRUE(res == expected);
}

TEST(SortCommandTest, SortTestEmpty) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    SortCommand sort_command(filesystem, "size");
    EXPECT_EQ(sort_command.execute(), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(), "OK");
    auto res = sort_command.execute();
    EXPECT_EQ(res, "OK:\n");
}

TEST(FreeCommandTest, FreeTest) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "data.bin", 5);
    CreateCommand create_test2(filesystem, "temp.log", 2);
    CreateCommand create_test3(filesystem, "report.txt", 3);
    FreeCommand free_command(filesystem);
    EXPECT_EQ(free_command.execute(), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the report.txt has been added");
    auto res = free_command.execute();
    std::string expected = "OK:\nOccupied: 10\nFree: 0\n";
    EXPECT_TRUE(res == expected);
}

TEST(FreeCommandTest, FreeTest2) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "data.bin", 5);
    CreateCommand create_test2(filesystem, "temp.log", 2);
    CreateCommand create_test3(filesystem, "report.txt", 3);
    DeleteCommand delete_test1(filesystem, "temp.log");
    FreeCommand free_command(filesystem);
    EXPECT_EQ(free_command.execute(), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the report.txt has been added");
    EXPECT_EQ(delete_test1.execute(), "OK: the temp.log has been removed");
    auto res = free_command.execute();
    std::string expected = "OK:\n1. 6 2\nOccupied: 8\nFree: 2\n";
    EXPECT_TRUE(res == expected);
}

TEST(FreeCommandTest, FreeTest3) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "data.bin", 5);
    CreateCommand create_test2(filesystem, "temp.log", 2);
    CreateCommand create_test3(filesystem, "report.txt", 3);
    DeleteCommand delete_test1(filesystem, "data.bin");
    DeleteCommand delete_test2(filesystem, "report.txt");
    FreeCommand free_command(filesystem);
    EXPECT_EQ(free_command.execute(), ERROR + ": the file system has not been initialized");
    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the data.bin has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the temp.log has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the report.txt has been added");
    EXPECT_EQ(delete_test1.execute(), "OK: the data.bin has been removed");
    EXPECT_EQ(delete_test2.execute(), "OK: the report.txt has been removed");
    auto res = free_command.execute();
    std::string expected = "OK:\n1. 1 5\n2. 8 3\nOccupied: 2\nFree: 8\n";
    EXPECT_TRUE(res == expected);
}

TEST(FreeCommandTest, FreeTest4) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    FreeCommand free_command(filesystem);
    EXPECT_EQ(init_command.execute(), "OK");
    auto res = free_command.execute();
    std::string expected = "OK:\n1. 1 10\nOccupied: 0\nFree: 10\n";
    EXPECT_TRUE(res == expected);
}

TEST(HelpCommandTest, HelpTest) {
    FileSystem filesystem;
    HelpCommand help_command(filesystem, "");
    HelpCommand help_command1(filesystem, "init");
    HelpCommand help_command2(filesystem, "create");
    HelpCommand help_command3(filesystem, "delete");
    HelpCommand help_command4(filesystem, "rename");
    HelpCommand help_command5(filesystem, "copy");
    HelpCommand help_command6(filesystem, "move");
    HelpCommand help_command7(filesystem, "add");
    HelpCommand help_command8(filesystem, "squeeze");
    HelpCommand help_command9(filesystem, "sort");
    HelpCommand help_command10(filesystem, "free");
    HelpCommand help_command11(filesystem, "vol");
    HelpCommand help_command12(filesystem, "exit");
    HelpCommand help_command13(filesystem, "help");
    HelpCommand help_command14(filesystem, "dir");
    EXPECT_EQ(help_command.execute(), OK + "\n" + HELP_FULL);
    EXPECT_EQ(help_command1.execute(), OK + "\n" + HELP_INIT);
    EXPECT_EQ(help_command2.execute(), OK + "\n" + HELP_CREATE);
    EXPECT_EQ(help_command3.execute(), OK + "\n" + HELP_DELETE);
    EXPECT_EQ(help_command4.execute(), OK + "\n" + HELP_RENAME);
    EXPECT_EQ(help_command5.execute(), OK + "\n" + HELP_COPY);
    EXPECT_EQ(help_command6.execute(), OK + "\n" + HELP_MOVE);
    EXPECT_EQ(help_command7.execute(), OK + "\n" + HELP_ADD);
    EXPECT_EQ(help_command8.execute(), OK + "\n" + HELP_SQUEEZE);
    EXPECT_EQ(help_command9.execute(), OK + "\n" + HELP_SORT);
    EXPECT_EQ(help_command10.execute(), OK + "\n" + HELP_FREE);
    EXPECT_EQ(help_command11.execute(), OK + "\n" + HELP_VOL);
    EXPECT_EQ(help_command12.execute(), OK + "\n" + HELP_EXIT);
    EXPECT_EQ(help_command13.execute(), OK + "\n" + HELP_HELP);
    EXPECT_EQ(help_command14.execute(), OK + "\n" + HELP_DIR);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}