#include <gtest/gtest.h>

#include "data_loader.h"

TEST(DataLoaderTest, DateConstructorValidInput) {
  EXPECT_NO_THROW({
    Date date("22/12/2022");
    EXPECT_EQ(date.getYear(), 2022);
    EXPECT_EQ(date.getMonth(), 12);
    EXPECT_EQ(date.getDay(), 22);
  });
}

TEST(DataLoaderTest, DateConstructorInvalidFormat) {
  EXPECT_THROW({ Date date("2022-12-22"); }, std::invalid_argument);
}

TEST(DataLoaderTest, DateConstructorInvalidValues) {
  EXPECT_THROW({ Date date("32/13/2022"); }, std::invalid_argument);
}

TEST(DataLoaderTest, DataLoaderConstructor) {
  EXPECT_NO_THROW({ DataLoader loader("test_file.txt"); });
}
