#include <gtest/gtest.h>

#include "block_ptr.h"
#include "managers.h"

TEST(BlockPtrTest, BlockPtrConstructor) {
  EXPECT_NO_THROW({
    FileManager fileManager("test_file.db", true);
    BlockPtr BlockPtr1 = fileManager.newPtr();
    BlockPtr BlockPtr2 = fileManager.newPtr();
    BlockPtr BlockPtr3 = fileManager.newPtr();
    BlockData block1 = BlockPtr1.load();
    BlockData block2 = BlockPtr2.load();
    BlockData block3 = BlockPtr3.load();
    // EXPECT_EQ(block1[0], 0);
    // EXPECT_EQ(block2[0], 0);
    // EXPECT_EQ(block3[0], 0);
    block2[0] = 'H';
    block2[1] = 'e';
    block2[2] = 'l';
    block2[3] = 'l';
    block2[4] = 'o';
    block2[5] = ' ';
    block2[6] = 'W';
    BlockPtr2.store(block2);
    const BlockData& new_block2 = BlockPtr2.load();
    EXPECT_EQ(new_block2[0], 'H');
    EXPECT_EQ(new_block2[1], 'e');
    EXPECT_EQ(new_block2[2], 'l');
    EXPECT_EQ(new_block2[3], 'l');
    EXPECT_EQ(new_block2[4], 'o');
    EXPECT_EQ(new_block2[5], ' ');
    EXPECT_EQ(fileManager.getPtrs().size(), 3);
    EXPECT_EQ(fileManager.getPtrs()[1].load()[0], 'H');
    remove("test_file.db");
  });
};

TEST(BlockPtrTest, BlockPtrSave) {
  EXPECT_NO_THROW({
    FileManager fileManager("test_file.db", true);
    BlockPtr BlockPtr1 = fileManager.newPtr();
    BlockPtr BlockPtr2 = fileManager.newPtr();
    BlockPtr BlockPtr3 = fileManager.newPtr();
    BlockData block1 = BlockPtr1.load();
    BlockData block2 = BlockPtr2.load();
    BlockData block3 = BlockPtr3.load();
    block2[0] = 'H';
    block2[1] = 'e';
    block2[2] = 'l';
    block2[3] = 'l';
    block2[4] = 'o';
    block2[5] = ' ';
    block2[6] = 'W';
    BlockPtr2.store(block2);
    BlockData new_block2 = BlockPtr2.load();
    EXPECT_EQ(new_block2[0], 'H');
    EXPECT_EQ(new_block2[1], 'e');
    EXPECT_EQ(new_block2[2], 'l');
    EXPECT_EQ(new_block2[3], 'l');
    EXPECT_EQ(new_block2[4], 'o');
    EXPECT_EQ(new_block2[5], ' ');
    EXPECT_EQ(fileManager.getPtrs().size(), 3);
    EXPECT_EQ(fileManager.getPtrs()[1].load()[0], 'H');

    Byte* bytes = BlockPtr2.getBytes();
    // BlockPtr* BlockPtr4 = BlockPtr::fromBytes(bytes);

    // BlockData new_block4 = BlockPtr4->load();
    // EXPECT_EQ(new_block4[0], 'H');
    // EXPECT_EQ(new_block4[1], 'e');
    // EXPECT_EQ(new_block4[2], 'l');
    // EXPECT_EQ(new_block4[3], 'l');
    // EXPECT_EQ(new_block4[4], 'o');
    // EXPECT_EQ(new_block4[5], ' ');

    BlockPtr BlockPtr5 = fileManager.getPtr(bytes);
    BlockData new_block5 = BlockPtr5.load();
    EXPECT_EQ(new_block5[0], 'H');
    EXPECT_EQ(new_block5[1], 'e');
    EXPECT_EQ(new_block5[2], 'l');
    EXPECT_EQ(new_block5[3], 'l');
    EXPECT_EQ(new_block5[4], 'o');
    EXPECT_EQ(new_block5[5], ' ');

    remove("test_file.db");
  });
};
