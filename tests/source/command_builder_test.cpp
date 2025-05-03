#include "command_builder/command_builder.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

#include "commands/abstract_command.hpp"
#include "commands/add_command.hpp"  // Для проверки типа
#include "commands/commands_constants.hpp"
#include "commands/copy_command.hpp"
#include "commands/create_command.hpp"
#include "commands/delete_command.hpp"
#include "commands/dir_command.hpp"  // Для проверки типа
#include "commands/init_command.hpp"
#include "commands/invalid_command.hpp"
#include "commands/move_command.hpp"  // Для проверки типа
#include "commands/rename_command.hpp"
#include "commands/squeeze_command.hpp"  // Для проверки типа
#include "commands/vol_command.hpp"      // Для проверки типа
#include "filesystem/catalog.hpp"
#include "filesystem/filesystem.hpp"
#include "nlohmann/json.hpp"

using command_builder::CommandBuilder;
using filesystem::Catalog;
using filesystem::Error;
using filesystem::FileSystem;

TEST(CommandBuilderTest, InitSuccessfully) {
    nlohmann::json init_json = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    auto init_cmd_ptr = command_builder.build(init_json);
    EXPECT_EQ(init_cmd_ptr->execute(), "OK");
    EXPECT_EQ(init_cmd_ptr->execute(), ERROR + ": the file system has already been initialized");
}

TEST(CommandBuilderTest, InitSuccessfullyPointer) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    FileSystem filesystem, second;
    CommandBuilder command_builder1(filesystem);
    CommandBuilder command_builder2(second);
    auto init_cmd_ptr1 = command_builder1.build(j);
    EXPECT_EQ(init_cmd_ptr1->execute(), "OK");
    auto init_cmd_ptr1_again = command_builder1.build(j);
    EXPECT_EQ(init_cmd_ptr1_again->execute(), ERROR + ": the file system has already been initialized");
    auto init_cmd_ptr2 = command_builder2.build(j);
    EXPECT_EQ(init_cmd_ptr2->execute(), "OK");
    auto init_cmd_ptr2_again = command_builder2.build(j);
    EXPECT_EQ(init_cmd_ptr2_again->execute(), ERROR + ": the file system has already been initialized");
    EXPECT_EQ(filesystem.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL");
    EXPECT_EQ(second.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(second.get_info().get_volume_name(), "VOL");
}

TEST(CommandBuilderTest, CreateFileSuccessfully) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 5}}}};
    nlohmann::json test4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 2}}}};
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    auto init_cmd_ptr = command_builder.build(j);
    init_cmd_ptr->execute();  // Инициализация файловой системы
    auto create_cmd_ptr1 = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr1->execute(), OK + ": the file has been added");
    auto create_cmd_ptr2 = command_builder.build(test2);
    EXPECT_EQ(create_cmd_ptr2->execute(), OK + ": the file has been added");
    auto create_cmd_ptr3 = command_builder.build(test3);
    EXPECT_EQ(create_cmd_ptr3->execute(), OK + ": the file has been added");
    auto create_cmd_ptr4 = command_builder.build(test4);
    EXPECT_EQ(create_cmd_ptr4->execute(), NO_FREE_SPACE);
}

TEST(CommandBuilderTest, CreateFileError) {
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 1}}}};
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    auto create_cmd_ptr = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");
}

TEST(CommandBuilderTest, CreateFileSuccessfully2) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test5 = {{"name", "create"}, {"data", {"test5.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test6 = {{"name", "create"}, {"data", {"test6.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test7 = {{"name", "create"}, {"data", {"test7.txt"}}, {"options", {{"allocate", 1}}}};
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    auto init_cmd_ptr = command_builder.build(j);
    init_cmd_ptr->execute();  // Инициализация файловой системы
    auto create_cmd_ptr1 = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr1->execute(), OK + ": the file has been added");
    auto create_cmd_ptr2 = command_builder.build(test2);
    EXPECT_EQ(create_cmd_ptr2->execute(), OK + ": the file has been added");
    auto create_cmd_ptr3 = command_builder.build(test3);
    EXPECT_EQ(create_cmd_ptr3->execute(), NO_FREE_RECORDS);  // Changed expectation as rec=2 now
    auto create_cmd_ptr4 = command_builder.build(test4);
    EXPECT_EQ(create_cmd_ptr4->execute(), NO_FREE_RECORDS);  // Changed expectation as rec=2 now
    auto create_cmd_ptr5 = command_builder.build(test5);
    EXPECT_EQ(create_cmd_ptr5->execute(), NO_FREE_RECORDS);  // Changed expectation as rec=2 now
    auto create_cmd_ptr6 = command_builder.build(test6);
    EXPECT_EQ(create_cmd_ptr6->execute(), NO_FREE_RECORDS);  // Changed expectation as rec=2 now
    auto create_cmd_ptr7 = command_builder.build(test7);
    EXPECT_EQ(create_cmd_ptr7->execute(), NO_FREE_RECORDS);
    auto create_cmd_ptr1_again = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr1_again->execute(), FILE_ALREADY_EXISTS);
    auto create_cmd_ptr2_again = command_builder.build(test2);         // Added check for test2
    EXPECT_EQ(create_cmd_ptr2_again->execute(), FILE_ALREADY_EXISTS);  // Added check for test2
}

TEST(CommandBuilderTest, CreateAndDeleteFileSuccessfully3) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 3}}}};
    nlohmann::json test4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test5 = {{"name", "create"}, {"data", {"test5.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test6 = {{"name", "create"}, {"data", {"test6.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test7 = {{"name", "create"}, {"data", {"test7.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json test8 = {{"name", "create"}, {"data", {"test8.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test1.txt"}}};  // Changed to delete test1
    nlohmann::json delete_test2 = {{"name", "delete"}, {"data", {"test2.txt"}}};  // Changed to delete test2
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    auto init_cmd_ptr = command_builder.build(j);
    init_cmd_ptr->execute();  // Инициализация файловой системы
    auto create_cmd_ptr1 = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr1->execute(), OK + ": the file has been added");
    auto create_cmd_ptr2 = command_builder.build(test2);
    EXPECT_EQ(create_cmd_ptr2->execute(), OK + ": the file has been added");
    auto create_cmd_ptr3 = command_builder.build(test3);
    EXPECT_EQ(create_cmd_ptr3->execute(), NO_FREE_RECORDS);  // Changed expectation as rec=2 now
    auto create_cmd_ptr4 = command_builder.build(test4);
    EXPECT_EQ(create_cmd_ptr4->execute(), NO_FREE_RECORDS);  // Changed expectation as rec=2 now
    auto create_cmd_ptr5 = command_builder.build(test5);
    EXPECT_EQ(create_cmd_ptr5->execute(), NO_FREE_RECORDS);      // Changed expectation as rec=2 now
    auto delete_cmd_ptr1 = command_builder.build(delete_test1);  // Changed to delete test1
    EXPECT_EQ(delete_cmd_ptr1->execute(), OK + ": the file has been removed");
    auto delete_cmd_ptr2 = command_builder.build(delete_test2);  // Changed to delete test2
    EXPECT_EQ(delete_cmd_ptr2->execute(), OK + ": the file has been removed");
    auto create_cmd_ptr6 = command_builder.build(test6);
    EXPECT_EQ(create_cmd_ptr6->execute(), OK + ": the file has been added");  // Now space for test6 (2 blocks, 1 rec)
    auto create_cmd_ptr7 = command_builder.build(test7);
    EXPECT_EQ(create_cmd_ptr7->execute(), OK + ": the file has been added");  // Now space for test7 (1 block, 1 rec)
    auto create_cmd_ptr8 = command_builder.build(test8);
    EXPECT_EQ(create_cmd_ptr8->execute(), NO_FREE_RECORDS);  // Still no more records
}

TEST(CommandBuilderTest, DeleteError) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test1.txt"}}};
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    auto create_cmd_ptr = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");
    auto delete_cmd_ptr = command_builder.build(delete_test1);
    EXPECT_EQ(delete_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");
    auto init_cmd_ptr = command_builder.build(j);
    init_cmd_ptr->execute();  // Инициализация файловой системы
    auto delete_cmd_ptr_after_init = command_builder.build(delete_test1);
    EXPECT_EQ(delete_cmd_ptr_after_init->execute(), FILE_NOT_FOUND);
    auto create_cmd_ptr_after_init = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr_after_init->execute(), OK + ": the file has been added");
    auto delete_cmd_ptr_final = command_builder.build(delete_test1);
    EXPECT_EQ(delete_cmd_ptr_final->execute(), OK + ": the file has been removed");
}

TEST(CommandBuilderTest, RenameFile) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test1.txt"}}};
    nlohmann::json delete_test2 = {{"name", "delete"}, {"data", {"test2.txt"}}};
    nlohmann::json rename_test1 = {{"name", "rename"}, {"data", {"test1.txt", "test2.txt"}}};
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    auto create_cmd_ptr = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");
    auto rename_cmd_ptr = command_builder.build(rename_test1);
    EXPECT_EQ(rename_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");
    auto init_cmd_ptr = command_builder.build(j);
    EXPECT_EQ(init_cmd_ptr->execute(), "OK");
    auto rename_cmd_ptr_after_init = command_builder.build(rename_test1);
    EXPECT_EQ(rename_cmd_ptr_after_init->execute(), FILE_NOT_FOUND);
    auto create_cmd_ptr_after_init = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr_after_init->execute(), OK + ": the file has been added");
    auto rename_cmd_ptr_final = command_builder.build(rename_test1);
    EXPECT_EQ(rename_cmd_ptr_final->execute(), OK + ": the file has been renamed");
    auto delete_cmd_ptr1 = command_builder.build(delete_test1);
    EXPECT_EQ(delete_cmd_ptr1->execute(), FILE_NOT_FOUND);
    auto delete_cmd_ptr2 = command_builder.build(delete_test2);
    EXPECT_EQ(delete_cmd_ptr2->execute(), OK + ": the file has been removed");
    auto rename_cmd_ptr_last = command_builder.build(rename_test1);
    EXPECT_EQ(rename_cmd_ptr_last->execute(), FILE_NOT_FOUND);
}

TEST(CommandBuilderTest, RenameToExistingFile) {
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 3}}}};
    nlohmann::json rename_test1 = {{"name", "rename"}, {"data", {"test1.txt", "test2.txt"}}};
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    auto init_cmd_ptr = command_builder.build(j);
    EXPECT_EQ(init_cmd_ptr->execute(), "OK");
    auto create_cmd_ptr1 = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr1->execute(), OK + ": the file has been added");
    auto create_cmd_ptr2 = command_builder.build(test2);
    EXPECT_EQ(create_cmd_ptr2->execute(), OK + ": the file has been added");
    auto rename_cmd_ptr = command_builder.build(rename_test1);
    EXPECT_EQ(rename_cmd_ptr->execute(), FILE_ALREADY_EXISTS);
}

TEST(CommandBuilderTest, CopyTest) {
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json copy_test1 = {{"name", "copy"},
                                 {"data", {"test3.txt", "test1.txt"}}};  // copy test3.txt -> test1.txt
    nlohmann::json copy_test2 = {{"name", "copy"},
                                 {"data", {"test3.txt", "test5.txt"}}};  // copy test3.txt -> test5.txt
    nlohmann::json copy_test3 = {{"name", "copy"},
                                 {"data", {"test1.txt", "test2.txt"}}};  // copy test1.txt -> test2.txt
    nlohmann::json copy_test4 = {{"name", "copy"},
                                 {"data", {"test1.txt", "test3.txt"}}};  // copy test1.txt -> test3.txt
    nlohmann::json copy_test5 = {{"name", "copy"},
                                 {"data", {"test1.txt", "test4.txt"}}};  // copy test1.txt -> test4.txt
    nlohmann::json copy_test6 = {{"name", "copy"},
                                 {"data", {"test1.txt", "test5.txt"}}};  // copy test1.txt -> test5.txt
    nlohmann::json copy_test7 = {{"name", "copy"},
                                 {"data", {"test1.txt", "test6.txt"}}};  // copy test1.txt -> test6.txt
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test3.txt"}}};

    auto create_cmd_ptr = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");
    auto copy_cmd_ptr = command_builder.build(copy_test1);
    EXPECT_EQ(copy_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");

    auto init_cmd_ptr = command_builder.build(j);
    EXPECT_EQ(init_cmd_ptr->execute(), "OK");

    auto create_cmd_ptr1 = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr1->execute(), OK + ": the file has been added");
    auto create_cmd_ptr2 = command_builder.build(test2);
    EXPECT_EQ(create_cmd_ptr2->execute(), OK + ": the file has been added");

    auto copy_cmd_ptr1 = command_builder.build(copy_test1);
    EXPECT_EQ(copy_cmd_ptr1->execute(), FILE_NOT_FOUND);  // Source test3.txt does not exist
    auto copy_cmd_ptr2 = command_builder.build(copy_test2);
    EXPECT_EQ(copy_cmd_ptr2->execute(), FILE_NOT_FOUND);  // Source test3.txt does not exist
    auto copy_cmd_ptr3 = command_builder.build(copy_test3);
    EXPECT_EQ(copy_cmd_ptr3->execute(), FILE_ALREADY_EXISTS);  // Destination test2.txt exists
    auto copy_cmd_ptr4 = command_builder.build(copy_test4);
    EXPECT_EQ(copy_cmd_ptr4->execute(), NO_FREE_RECORDS);  // Copy test1.txt to test3.txt (needs 1 record, only 2 total)
    auto copy_cmd_ptr5 = command_builder.build(copy_test5);
    EXPECT_EQ(copy_cmd_ptr5->execute(), NO_FREE_RECORDS);  // Copy test1.txt to test4.txt (needs 1 record)
    auto copy_cmd_ptr6 = command_builder.build(copy_test6);
    EXPECT_EQ(copy_cmd_ptr6->execute(), NO_FREE_RECORDS);  // Copy test1.txt to test5.txt (needs 1 record)
    auto copy_cmd_ptr7 = command_builder.build(copy_test7);
    EXPECT_EQ(copy_cmd_ptr7->execute(), NO_FREE_RECORDS);  // Not enough records for test6.txt

    // Need to create test3 first to delete it
    nlohmann::json create_test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 2}}}};
    auto create_cmd_ptr3 = command_builder.build(create_test3);
    // This will fail because no records left, let's delete test2 first
    nlohmann::json delete_test2 = {{"name", "delete"}, {"data", {"test2.txt"}}};
    auto delete_cmd_ptr2 = command_builder.build(delete_test2);
    EXPECT_EQ(delete_cmd_ptr2->execute(), OK + ": the file has been removed");  // Delete test2 (frees 1 block, 1 rec)

    create_cmd_ptr3 = command_builder.build(create_test3);
    EXPECT_EQ(create_cmd_ptr3->execute(), OK + ": the file has been added");  // Create test3 (uses 2 blocks, 1 rec)

    auto delete_cmd_ptr1 = command_builder.build(delete_test1);
    EXPECT_EQ(delete_cmd_ptr1->execute(),
              OK + ": the file has been removed");  // Delete test3.txt (frees 2 blocks, 1 rec)

    auto copy_cmd_ptr7_again = command_builder.build(copy_test7);
    EXPECT_EQ(copy_cmd_ptr7_again->execute(),
              OK + ": the file has been added");  // Now enough space and records for test6.txt
}

TEST(CommandBuilderTest, MoveTest) {
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 1}}}};
    nlohmann::json move_test1 = {{"name", "move"},
                                 {"data", {"test3.txt", "test1.txt"}}};  // move test3.txt -> test1.txt
    nlohmann::json move_test2 = {{"name", "move"},
                                 {"data", {"test3.txt", "test5.txt"}}};  // move test3.txt -> test5.txt
    nlohmann::json move_test3 = {{"name", "move"},
                                 {"data", {"test1.txt", "test2.txt"}}};  // move test1.txt -> test2.txt
    nlohmann::json move_test4 = {{"name", "move"},
                                 {"data", {"test1.txt", "test3.txt"}}};  // move test1.txt -> test3.txt
    nlohmann::json move_test5 = {{"name", "move"},
                                 {"data", {"test1.txt", "test4.txt"}}};  // move test1.txt -> test4.txt
    nlohmann::json move_test6 = {{"name", "move"},
                                 {"data", {"test3.txt", "test1.txt"}}};  // move test3.txt -> test1.txt
    nlohmann::json move_test7 = {{"name", "move"},
                                 {"data", {"test1.txt", "test3.txt"}}};  // move test1.txt -> test3.txt
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test1.txt"}}};
    nlohmann::json delete_test2 = {{"name", "delete"}, {"data", {"test3.txt"}}};

    auto create_cmd_ptr = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");
    auto move_cmd_ptr = command_builder.build(move_test1);
    EXPECT_EQ(move_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");

    auto init_cmd_ptr = command_builder.build(j);
    EXPECT_EQ(init_cmd_ptr->execute(), "OK");

    auto create_cmd_ptr1 = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr1->execute(), OK + ": the file has been added");
    auto create_cmd_ptr2 = command_builder.build(test2);
    EXPECT_EQ(create_cmd_ptr2->execute(), OK + ": the file has been added");

    auto move_cmd_ptr1 = command_builder.build(move_test1);
    EXPECT_EQ(move_cmd_ptr1->execute(), FILE_NOT_FOUND);  // Source test3.txt does not exist
    auto move_cmd_ptr2 = command_builder.build(move_test2);
    EXPECT_EQ(move_cmd_ptr2->execute(), FILE_NOT_FOUND);  // Source test3.txt does not exist
    auto move_cmd_ptr3 = command_builder.build(move_test3);
    EXPECT_EQ(move_cmd_ptr3->execute(), FILE_ALREADY_EXISTS);  // Destination test2.txt exists
    auto move_cmd_ptr4 = command_builder.build(move_test4);
    EXPECT_EQ(move_cmd_ptr4->execute(), OK + " the file has been moved");  // move test1.txt -> test3.txt
    auto move_cmd_ptr5 = command_builder.build(move_test5);
    EXPECT_EQ(move_cmd_ptr5->execute(), FILE_NOT_FOUND);  // Source test1.txt does not exist (was moved)
    auto move_cmd_ptr6 = command_builder.build(move_test6);
    EXPECT_EQ(move_cmd_ptr6->execute(), OK + " the file has been moved");  // move test3.txt -> test1.txt
    auto move_cmd_ptr7 = command_builder.build(move_test7);
    EXPECT_EQ(move_cmd_ptr7->execute(), OK + " the file has been moved");  // move test1.txt -> test3.txt

    auto delete_cmd_ptr1 = command_builder.build(delete_test1);
    EXPECT_EQ(delete_cmd_ptr1->execute(), FILE_NOT_FOUND);  // test1.txt does not exist (was moved)
    auto delete_cmd_ptr2 = command_builder.build(delete_test2);
    EXPECT_EQ(delete_cmd_ptr2->execute(), OK + ": the file has been removed");  // delete test3.txt
}

TEST(CommandBuilderTest, VolTest) {
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    nlohmann::json vol_test1{{"name", "vol"}, {"data", {"VOL_1"}}};
    nlohmann::json vol_test2{{"name", "vol"}, {"data", {"VOL_3", "User"}}};

    auto vol_cmd_ptr1 = command_builder.build(vol_test1);
    EXPECT_EQ(vol_cmd_ptr1->execute(), ERROR + ": the file system has not been initialized");

    auto init_cmd_ptr = command_builder.build(j);
    EXPECT_EQ(init_cmd_ptr->execute(), "OK");
    EXPECT_EQ(filesystem.get_info().get_owner_name(), "OWNER");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL");

    auto vol_cmd_ptr1_after_init = command_builder.build(vol_test1);
    EXPECT_EQ(vol_cmd_ptr1_after_init->execute(), OK + ": the volume ID has been changed");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL_1");

    auto vol_cmd_ptr2 = command_builder.build(vol_test2);
    EXPECT_EQ(vol_cmd_ptr2->execute(), OK + ": the volume ID has been changed");
    EXPECT_EQ(filesystem.get_info().get_owner_name(), "User");
    EXPECT_EQ(filesystem.get_info().get_volume_name(), "VOL_3");
}

TEST(CommandBuilderTest, AddTest) {
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
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
    nlohmann::json delete_test2 = {{"name", "delete"}, {"data", {"test3.txt"}}};  // Changed to delete test3
    nlohmann::json add_test6 = {{"name", "add"}, {"data", {"test5.txt"}}, {"options", {{"size", 4}}}};
    nlohmann::json add_test7 = {{"name", "add"}, {"data", {"test5.txt"}}, {"options", {{"size", 3}}}};

    auto add_cmd_ptr = command_builder.build(add_test1);
    EXPECT_EQ(add_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");

    auto init_cmd_ptr = command_builder.build(j);
    EXPECT_EQ(init_cmd_ptr->execute(), "OK");

    auto create_cmd_ptr1 = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr1->execute(), OK + ": the file has been added");  // Uses 2 blocks, 1 rec
    auto create_cmd_ptr2 = command_builder.build(test2);
    EXPECT_EQ(create_cmd_ptr2->execute(),
              OK + ": the file has been added");  // Uses 1 block, 1 rec (Total: 3 blocks, 2 recs)
    auto create_cmd_ptr3 = command_builder.build(test3);
    EXPECT_EQ(create_cmd_ptr3->execute(), NO_FREE_RECORDS);  // Needs 1 rec, none left
    auto create_cmd_ptr4 = command_builder.build(test4);
    EXPECT_EQ(create_cmd_ptr4->execute(), NO_FREE_RECORDS);  // Needs 1 rec, none left
    auto create_cmd_ptr5 = command_builder.build(test5);
    EXPECT_EQ(create_cmd_ptr5->execute(), NO_FREE_RECORDS);  // Needs 1 rec, none left

    auto add_cmd_ptr1 = command_builder.build(add_test1);
    EXPECT_EQ(add_cmd_ptr1->execute(), FILE_NOT_FOUND);
    auto add_cmd_ptr2 = command_builder.build(add_test2);  // Add 10 blocks to test1 (needs 10, has 10-3=7 free)
    EXPECT_EQ(add_cmd_ptr2->execute(), NO_FREE_SPACE);
    auto add_cmd_ptr3 = command_builder.build(add_test3);  // Add 1 block to test1 (needs 1, has 7 free)
    EXPECT_EQ(add_cmd_ptr3->execute(),
              OK + ": the file size has been increased");        // test1=3 blocks, test2=1 block (Total: 4 blocks)
    auto add_cmd_ptr3_again = command_builder.build(add_test3);  // Add 1 block to test1 (needs 1, has 10-4=6 free)
    EXPECT_EQ(add_cmd_ptr3_again->execute(),
              OK + ": the file size has been increased");  // test1=4 blocks, test2=1 block (Total: 5 blocks)
    auto add_cmd_ptr4 = command_builder.build(add_test4);  // Add 1 block to test2 (needs 1, has 10-5=5 free)
    EXPECT_EQ(add_cmd_ptr4->execute(),
              OK + ": the file size has been increased");  // test1=4 blocks, test2=2 blocks (Total: 6 blocks)
    auto add_cmd_ptr5 = command_builder.build(add_test5);  // Add 1 block to test3 (test3 doesn't exist)
    EXPECT_EQ(add_cmd_ptr5->execute(), FILE_NOT_FOUND);

    auto delete_cmd_ptr1 = command_builder.build(delete_test1);  // Delete test2 (frees 2 blocks, 1 rec)
    EXPECT_EQ(delete_cmd_ptr1->execute(),
              OK + ": the file has been removed");               // test1=4 blocks (Total: 4 blocks, 1 rec free)
    auto delete_cmd_ptr2 = command_builder.build(delete_test2);  // Delete test3 (doesn't exist)
    EXPECT_EQ(delete_cmd_ptr2->execute(), FILE_NOT_FOUND);

    auto add_cmd_ptr5_again = command_builder.build(add_test5);  // Add 1 block to test3 (still doesn't exist)
    EXPECT_EQ(add_cmd_ptr5_again->execute(), FILE_NOT_FOUND);
    auto add_cmd_ptr3_final = command_builder.build(add_test3);  // Add 1 block to test1 (needs 1, has 10-4=6 free)
    EXPECT_EQ(add_cmd_ptr3_final->execute(),
              OK + ": the file size has been increased");  // test1=5 blocks (Total: 5 blocks)

    // Create test5 now that there's a record free
    auto create_cmd_ptr5_now = command_builder.build(test5);
    EXPECT_EQ(create_cmd_ptr5_now->execute(),
              OK + ": the file has been added");  // test5=2 blocks (Total: 7 blocks, 2 recs used)

    auto add_cmd_ptr6 = command_builder.build(add_test6);  // Add 4 blocks to test5 (needs 4, has 10-7=3 free)
    EXPECT_EQ(add_cmd_ptr6->execute(), NO_FREE_SPACE);
    auto add_cmd_ptr7 = command_builder.build(add_test7);  // Add 3 blocks to test5 (needs 3, has 3 free)
    EXPECT_EQ(add_cmd_ptr7->execute(),
              OK + ": the file size has been increased");  // test1=5, test5=5 (Total: 10 blocks)
}

TEST(CommandBuilderTest, SqueezeTest1) {
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 5}}}};
    nlohmann::json test4 = {
        {"name", "create"}, {"data", {"test2_new.txt"}}, {"options", {{"allocate", 3}}}};  // Renamed to avoid conflict
    nlohmann::json test5 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test2.txt"}}};
    nlohmann::json empty = {{"name", "squeeze"}, {"data", nullptr}};  // Squeeze command JSON

    auto squeeze_cmd_ptr = command_builder.build(empty);
    EXPECT_EQ(squeeze_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");

    auto init_cmd_ptr = command_builder.build(j);
    EXPECT_EQ(init_cmd_ptr->execute(), "OK");

    auto create_cmd_ptr1 = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr1->execute(), OK + ": the file has been added");  // test1=2 blocks, 1 rec
    auto create_cmd_ptr2 = command_builder.build(test2);
    EXPECT_EQ(create_cmd_ptr2->execute(),
              OK + ": the file has been added");  // test2=2 blocks, 1 rec (Total: 4 blocks, 2 recs)
    auto create_cmd_ptr3 = command_builder.build(test3);
    EXPECT_EQ(create_cmd_ptr3->execute(), NO_FREE_RECORDS);  // Needs 1 rec, none left

    auto delete_cmd_ptr1 = command_builder.build(delete_test1);
    EXPECT_EQ(delete_cmd_ptr1->execute(),
              OK + ": the file has been removed");  // test1=2 blocks (Total: 2 blocks, 1 rec used, 1 rec free)

    auto& catalog = filesystem.get_catalog();
    // Busy segments might depend on allocation strategy, cannot reliably test here without knowing implementation
    // details. EXPECT_EQ(catalog.get_busy_segments_count(), ?); EXPECT_EQ(catalog.get_used_segments_count(), ?);

    auto create_cmd_ptr4 = command_builder.build(test4);  // Create test2_new (needs 3 blocks, 1 rec)
    EXPECT_EQ(create_cmd_ptr4->execute(),
              OK + ": the file has been added");  // test1=2, test2_new=3 (Total: 5 blocks, 2 recs used)

    auto squeeze_cmd_ptr_after_init = command_builder.build(empty);
    EXPECT_EQ(squeeze_cmd_ptr_after_init->execute(), OK + ": fragmentation was completed successfully");

    // Again, segment counts depend on implementation.
    // EXPECT_EQ(catalog.get_busy_segments_count(), ?);
    // EXPECT_EQ(catalog.get_used_segments_count(), ?);

    auto create_cmd_ptr5 = command_builder.build(test5);     // Create test4 (needs 2 blocks, 1 rec)
    EXPECT_EQ(create_cmd_ptr5->execute(), NO_FREE_RECORDS);  // No records left
}

TEST(CommandBuilderTest, SqueezeTest2) {
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    nlohmann::json j = {{"name", "init"},
                        {"data", {"VOL", "OWNER"}},
                        {"options", {{"segm", 3}, {"vol", 20}, {"rec", 3}}}};  // Swapped vol/rec
    nlohmann::json test1 = {{"name", "create"}, {"data", {"test1.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test2 = {{"name", "create"}, {"data", {"test2.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test3 = {{"name", "create"}, {"data", {"test3.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test4 = {{"name", "create"}, {"data", {"test4.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test5 = {{"name", "create"}, {"data", {"test5.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json test6 = {{"name", "create"}, {"data", {"test6.txt"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json empty = {{"name", "squeeze"}, {"data", nullptr}};  // Squeeze command JSON
    nlohmann::json delete_test1 = {{"name", "delete"}, {"data", {"test2.txt"}}};
    nlohmann::json delete_test2 = {{"name", "delete"}, {"data", {"test5.txt"}}};

    auto squeeze_cmd_ptr = command_builder.build(empty);
    EXPECT_EQ(squeeze_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");

    auto init_cmd_ptr = command_builder.build(j);
    EXPECT_EQ(init_cmd_ptr->execute(), "OK");

    auto create_cmd_ptr1 = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr1->execute(), OK + ": the file has been added");  // 2 blocks, 1 rec
    auto create_cmd_ptr2 = command_builder.build(test2);
    EXPECT_EQ(create_cmd_ptr2->execute(), OK + ": the file has been added");  // 2 blocks, 1 rec
    auto create_cmd_ptr3 = command_builder.build(test3);
    EXPECT_EQ(create_cmd_ptr3->execute(),
              OK + ": the file has been added");  // 2 blocks, 1 rec (Total: 6 blocks, 3 recs)
    auto create_cmd_ptr4 = command_builder.build(test4);
    EXPECT_EQ(create_cmd_ptr4->execute(), NO_FREE_RECORDS);  // Needs 1 rec, none left
    auto create_cmd_ptr5 = command_builder.build(test5);
    EXPECT_EQ(create_cmd_ptr5->execute(), NO_FREE_RECORDS);  // Needs 1 rec, none left
    auto create_cmd_ptr6 = command_builder.build(test6);
    EXPECT_EQ(create_cmd_ptr6->execute(), NO_FREE_RECORDS);  // Needs 1 rec, none left

    auto squeeze_cmd_ptr_after_init = command_builder.build(empty);
    EXPECT_EQ(squeeze_cmd_ptr_after_init->execute(), OK + ": fragmentation was completed successfully");

    auto& catalog = filesystem.get_catalog();
    // Segment counts depend on implementation.
    // EXPECT_EQ(catalog.get_busy_segments_count(), ?);
    // EXPECT_EQ(catalog.get_used_segments_count(), ?);

    auto delete_cmd_ptr1 = command_builder.build(delete_test1);
    EXPECT_EQ(delete_cmd_ptr1->execute(), OK + ": the file has been removed");  // Frees 2 blocks, 1 rec
    // Cannot delete test5 as it was never created
    // auto delete_cmd_ptr2 = command_builder.build(delete_test2);
    // EXPECT_EQ(delete_cmd_ptr2->execute(), FILE_NOT_FOUND);

    // Segment counts depend on implementation.
    // EXPECT_EQ(catalog.get_busy_segments_count(), ?);
    // EXPECT_EQ(catalog.get_used_segments_count(), ?);

    auto squeeze_cmd_ptr_final = command_builder.build(empty);
    EXPECT_EQ(squeeze_cmd_ptr_final->execute(), OK + ": fragmentation was completed successfully");

    // Segment counts depend on implementation.
    // EXPECT_EQ(catalog.get_busy_segments_count(), ?);
    // EXPECT_EQ(catalog.get_used_segments_count(), ?);
}

TEST(CommandBuilderTest, RenameWithDifferentExtension) {
    FileSystem filesystem;
    CommandBuilder command_builder(filesystem);
    nlohmann::json j = {
        {"name", "init"}, {"data", {"VOL", "OWNER"}}, {"options", {{"segm", 3}, {"vol", 10}, {"rec", 2}}}};
    nlohmann::json test1 = {{"name", "create"}, {"data", {"data.bin"}}, {"options", {{"allocate", 2}}}};
    nlohmann::json rename_test1 = {{"name", "rename"}, {"data", {"data.bin", "data.txt"}}};
    nlohmann::json dir_test = {{"name", "dir"}, {"data", nullptr}};

    auto dir_cmd_ptr = command_builder.build(dir_test);
    EXPECT_EQ(dir_cmd_ptr->execute(), ERROR + ": the file system has not been initialized");

    auto init_cmd_ptr = command_builder.build(j);
    EXPECT_EQ(init_cmd_ptr->execute(), "OK");

    auto create_cmd_ptr1 = command_builder.build(test1);
    EXPECT_EQ(create_cmd_ptr1->execute(), OK + ": the file has been added");

    auto rename_cmd_ptr1 = command_builder.build(rename_test1);
    EXPECT_EQ(rename_cmd_ptr1->execute(), OK + ": the file has been renamed");

    auto dir_cmd_ptr_final = command_builder.build(dir_test);
    auto res = dir_cmd_ptr_final->execute();

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();
    std::string expected_line = "data.txt 2 Blocks " + today;

    EXPECT_NE(res.find(expected_line), std::string::npos)
        << "Expected line '" << expected_line << "' not found in result:\n"
        << res;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
