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

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
