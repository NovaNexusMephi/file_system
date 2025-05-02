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

    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the file has been added");
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

    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test4.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test5.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test6.execute(), "OK: the file has been added");
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

    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test4.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test5.execute(), "OK: the file has been added");

    EXPECT_EQ(delete_test3.execute(), "OK: the file has been removed");
    EXPECT_EQ(delete_test4.execute(), "OK: the file has been removed");

    EXPECT_EQ(create_test6.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test7.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test8.execute(), "OK: the file has been added");
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
    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(delete_test1.execute(), "OK: the file has been removed");
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
    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(rename_test1.execute(), "OK: the file has been renamed");
    EXPECT_EQ(delete_test1.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(delete_test2.execute(), "OK: the file has been removed");
    EXPECT_EQ(rename_test1.execute(), "FILE_NOT_FOUND");
}

TEST(RenameTestCommand, RenameToExistingFile) {
    FileSystem filesystem;
    InitCommand init_command(filesystem, "VOL", "OWNER", 3, 2, 10);
    CreateCommand create_test1(filesystem, "test1.txt", 2);
    CreateCommand create_test2(filesystem, "test2.txt", 3);
    RenameCommand rename_test1(filesystem, "test1.txt", "test2.txt");

    EXPECT_EQ(init_command.execute(), "OK");
    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the file has been added");
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
    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the file has been added");

    EXPECT_EQ(copy_test1.execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(copy_test2.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(copy_test3.execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(copy_test4.execute(), "OK: the file has been added");
    EXPECT_EQ(copy_test5.execute(), "OK: the file has been added");
    EXPECT_EQ(copy_test6.execute(), "OK: the file has been added");
    EXPECT_EQ(copy_test7.execute(), "NO_FREE_SPACE");

    EXPECT_EQ(delete_test1.execute(), "OK: the file has been removed");
    EXPECT_EQ(copy_test7.execute(), "OK: the file has been added");
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
    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the file has been added");

    EXPECT_EQ(move_test1.execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(move_test2.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(move_test3.execute(), "FILE_ALREADY_EXISTS");
    EXPECT_EQ(move_test4.execute(), "OK: the file has been moved");
    EXPECT_EQ(move_test5.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(move_test6.execute(), "OK: the file has been moved");
    EXPECT_EQ(move_test7.execute(), "OK: the file has been moved");

    EXPECT_EQ(delete_test1.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(delete_test2.execute(), "OK: the file has been removed");
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

    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test4.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test5.execute(), "OK: the file has been added");

    EXPECT_EQ(add_test1.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(add_test2.execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test3.execute(), "OK: the file size has been increased");
    EXPECT_EQ(add_test3.execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test4.execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test5.execute(), "NO_FREE_SPACE");

    EXPECT_EQ(delete_test1.execute(), "OK: the file has been removed");
    EXPECT_EQ(delete_test2.execute(), "OK: the file has been removed");

    EXPECT_EQ(add_test5.execute(), "FILE_NOT_FOUND");
    EXPECT_EQ(add_test3.execute(), "OK: the file size has been increased");
    EXPECT_EQ(add_test6.execute(), "NO_FREE_SPACE");
    EXPECT_EQ(add_test7.execute(), "OK: the file size has been increased");
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

    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the file has been added");
    EXPECT_EQ(delete_test1.execute(), "OK: the file has been removed");

    auto& catalog = filesystem.get_catalog();
    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);

    EXPECT_EQ(create_test4.execute(), "NO_FREE_SPACE");
    EXPECT_EQ(squeeze_command.execute(), "OK: fragmentation was completed successfully");

    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 1);

    EXPECT_EQ(create_test4.execute(), "OK: the file has been added");
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

    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test2.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test3.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test4.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test5.execute(), "OK: the file has been added");
    EXPECT_EQ(create_test6.execute(), "OK: the file has been added");

    EXPECT_EQ(squeeze_command.execute(), "OK: fragmentation was completed successfully");

    auto& catalog = filesystem.get_catalog();
    EXPECT_EQ(catalog.get_busy_segments_count(), 2);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);

    EXPECT_EQ(delete_test1.execute(), "OK: the file has been removed");
    EXPECT_EQ(delete_test2.execute(), "OK: the file has been removed");

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

    EXPECT_EQ(create_test1.execute(), "OK: the file has been added");
    EXPECT_EQ(rename_test1.execute(), "OK: the file has been renamed");

    auto res = dir_command.execute();
    std::string expected_line = "data.txt 2 Blocks " + today;

    EXPECT_NE(res.find(expected_line), std::string::npos)
        << "Expected line '" << expected_line << "' not found in result:\n"
        << res;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}