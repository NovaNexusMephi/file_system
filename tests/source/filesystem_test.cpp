#include "filesystem/filesystem.hpp"

#include <gtest/gtest.h>

#include "create_command.hpp"
#include "filesystem/catalog.hpp"
#include "init_command.hpp"

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

/*TEST(CatalogTest, CreateFileSuccessfully3) {
    Catalog catalog(3, 2, 10);

    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test2.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test3.txt", 3), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test4.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test5.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.remove("test3.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.remove("test4.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test6.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test7.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test8.txt", 1), Error::NO_ERROR);
}

TEST(CatalogTest, RenameFile) {
    Catalog catalog(3, 2, 10);
    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.rename("test1.txt", "test2.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.remove("test2.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.rename("test2.txt", "test1.txt"), Error::FILE_NOT_FOUND);
}

TEST(CatalogTest, RenameToExistingFile) {
    Catalog catalog(3, 2, 10);

    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test2.txt", 3), Error::NO_ERROR);

    EXPECT_EQ(catalog.rename("test1.txt", "test2.txt"), Error::FILE_ALREADY_EXISTS);
}

TEST(CatalogTest, RenameWithDifferentExtension) {
    Catalog catalog(3, 2, 10);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y");
    std::string today = ss.str();

    EXPECT_EQ(catalog.create("data.bin", 5), Error::NO_ERROR);
    EXPECT_EQ(catalog.rename("data.bin", "data.txt"), Error::NO_ERROR);

    std::vector<std::string> result = catalog.dir();
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "data.txt 5 Blocks " + today);
}

TEST(CatalogTest, RenameToSameName) {
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

TEST(CatalogTest, AddTest) {
    Catalog catalog(3, 2, 10);
    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test2.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test3.txt", 3), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test4.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test5.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.add("test123.txt", 1), Error::FILE_NOT_FOUND);
    EXPECT_EQ(catalog.add("test1.txt", 10), Error::NO_FREE_SPACE);
    EXPECT_EQ(catalog.add("test1.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.add("test1.txt", 1), Error::NO_FREE_SPACE);
    EXPECT_EQ(catalog.add("test2.txt", 1), Error::NO_FREE_SPACE);
    EXPECT_EQ(catalog.add("test3.txt", 1), Error::NO_FREE_SPACE);
    EXPECT_EQ(catalog.remove("test2.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.remove("test3.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.add("test3.txt", 1), Error::FILE_NOT_FOUND);
    EXPECT_EQ(catalog.add("test1.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.add("test5.txt", 4), Error::NO_FREE_SPACE);
    EXPECT_EQ(catalog.add("test5.txt", 3), Error::NO_ERROR);
}

TEST(CatalogTest, MoveTest) {
    Catalog catalog(3, 2, 10);
    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test2.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.move("test3.txt", "test1.txt"), Error::FILE_ALREADY_EXISTS);
    EXPECT_EQ(catalog.move("test3.txt", "test5.txt"), Error::FILE_NOT_FOUND);
    EXPECT_EQ(catalog.move("test1.txt", "test2.txt"), Error::FILE_ALREADY_EXISTS);
    EXPECT_EQ(catalog.move("test1.txt", "test3.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.move("test1.txt", "test4.txt"), Error::FILE_NOT_FOUND);
    EXPECT_EQ(catalog.move("test3.txt", "test1.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.move("test1.txt", "test3.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.remove("test1.txt"), Error::FILE_NOT_FOUND);
    EXPECT_EQ(catalog.remove("test3.txt"), Error::NO_ERROR);
}

TEST(CatalogTest, CopyTest) {
    Catalog catalog(3, 2, 10);
    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test2.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.copy("test3.txt", "test1.txt"), Error::FILE_ALREADY_EXISTS);
    EXPECT_EQ(catalog.copy("test3.txt", "test5.txt"), Error::FILE_NOT_FOUND);
    EXPECT_EQ(catalog.copy("test1.txt", "test2.txt"), Error::FILE_ALREADY_EXISTS);
    EXPECT_EQ(catalog.copy("test1.txt", "test3.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.copy("test1.txt", "test4.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.copy("test1.txt", "test5.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.copy("test1.txt", "test6.txt"), Error::NO_FREE_SPACE);
    EXPECT_EQ(catalog.remove("test3.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.copy("test1.txt", "test6.txt"), Error::NO_ERROR);
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

TEST(CatalogTest, SqueezeTest1) {
    Catalog catalog(3, 2, 10);
    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test2.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test3.txt", 5), Error::NO_ERROR);
    EXPECT_EQ(catalog.remove("test2.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);
    EXPECT_EQ(catalog.create("test2.txt", 3), Error::NO_FREE_SPACE);
    EXPECT_EQ(catalog.squeeze(), Error::NO_ERROR);
    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 1);
    EXPECT_EQ(catalog.create("test2.txt", 3), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test4.txt", 2), Error::NO_FREE_SPACE);
}

TEST(CatalogTest, SqueezeTest2) {
    Catalog catalog(3, 3, 20);
    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test2.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test3.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test4.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test5.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test6.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.squeeze(), Error::NO_ERROR);
    EXPECT_EQ(catalog.get_busy_segments_count(), 2);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);
    EXPECT_EQ(catalog.remove("test2.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.remove("test5.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.get_busy_segments_count(), 2);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);
    EXPECT_EQ(catalog.squeeze(), Error::NO_ERROR);
    EXPECT_EQ(catalog.get_busy_segments_count(), 1);
    EXPECT_EQ(catalog.get_used_segments_count(), 2);
}

TEST(FileSystem, InitTest) {
    FileSystem filesystem;
    EXPECT_EQ(filesystem.init("VOL", "OWNER", 1000000000072376370), Error::ERROR_EXCEPTION);
    EXPECT_EQ(filesystem.init("VOL", "OWNER", 1, 1, 1), Error::NO_ERROR);
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
}

TEST(FileSystem, VolTest) {
    FileSystem filesystem;
    filesystem.init("VOL", "OWNER", 3, 2, 10);
    EXPECT_EQ(filesystem.get_info()->get_owner_name(), "OWNER");
    EXPECT_EQ(filesystem.get_info()->get_volume_name(), "VOL");
    EXPECT_EQ(filesystem.vol("VOL_3", "User"), Error::NO_ERROR);
    EXPECT_EQ(filesystem.get_info()->get_owner_name(), "User");
    EXPECT_EQ(filesystem.get_info()->get_volume_name(), "VOL_3");
}*/

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
