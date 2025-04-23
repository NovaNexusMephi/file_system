#include <gtest/gtest.h>

#include "filesystem/catalog.hpp"

using filesystem::Catalog;
using filesystem::Error;

TEST(CatalogTest, CreateFileSuccessfully) {
    Catalog catalog(3, 2, 10);

    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test2.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test3.txt", 5), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test4.txt", 2), Error::NO_FREE_SPACE);
}

TEST(CatalogTest, CreateFileSuccessfully2) {
    Catalog catalog(3, 2, 10);

    EXPECT_EQ(catalog.create("test1.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test2.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test3.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test4.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test5.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test6.txt", 1), Error::NO_ERROR);
    EXPECT_EQ(catalog.create("test7.txt", 1), Error::NO_FREE_RECORDS);
}

TEST(CatalogTest, CreateFileSuccessfully3) {
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
    EXPECT_EQ(catalog.create("test8.txt", 1), Error::NO_FREE_SPACE);
}

TEST(CatalogTest, RenameFile) {
    Catalog catalog(3, 2, 10);
    EXPECT_EQ(catalog.create("test1.txt", 2), Error::NO_ERROR);
    EXPECT_EQ(catalog.rename("test1.txt", "test2.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.remove("test2.txt"), Error::NO_ERROR);
    EXPECT_EQ(catalog.rename("test2.txt", "test1.txt"), Error::FILE_NOT_FOUND);
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

    std::vector<std::string> expected = {
        "test1.txt 2 Blocks 2025-04-23 11:09:20",  
        "test2.txt 1 Blocks 2025-04-23 11:09:20"  
    };

    std::vector<std::string> result = catalog.dir();

    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        size_t pos = result[i].find("Blocks");
        EXPECT_EQ(pos, 12);
        pos = result[i].find("test" + std::to_string(i + 1) + ".txt");
        EXPECT_EQ(pos, 0);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
