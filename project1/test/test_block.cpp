#include <gtest/gtest.h>

#include "block.h"

TEST(BlockTest, BlockConstructor) {
  EXPECT_NO_THROW({
    Block block("Hello World!");
    EXPECT_EQ(block[0], 'H');
    EXPECT_EQ(block[1], 'e');
    EXPECT_EQ(block[2], 'l');
    EXPECT_EQ(block[3], 'l');
    EXPECT_EQ(block[4], 'o');
    EXPECT_EQ(block[5], ' ');
    EXPECT_EQ(block[6], 'W');
    EXPECT_EQ(block[7], 'o');
    EXPECT_EQ(block[8], 'r');
    EXPECT_EQ(block[9], 'l');
    EXPECT_EQ(block[10], 'd');
    EXPECT_EQ(block[11], '!');
    EXPECT_EQ(block[12], '\0');
  });
};
