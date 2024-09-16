#include <gtest/gtest.h>

#include "data_ptr.h"
#include "managers.h"

TEST(DataPtrTest, DataPtrConstructor) {
  EXPECT_NO_THROW({
    FileManager fileManager("test_file.db");
    DataPtr dataPtr1 = fileManager.newPtr();
    DataPtr dataPtr2 = fileManager.newPtr();
    DataPtr dataPtr3 = fileManager.newPtr();
    BlockData block1 = dataPtr1.load();
    BlockData block2 = dataPtr2.load();
    BlockData block3 = dataPtr3.load();
    EXPECT_EQ(block1[0], 0);
    EXPECT_EQ(block2[0], 0);
    EXPECT_EQ(block3[0], 0);
    block2[0] = 'H';
    block2[1] = 'e';
    block2[2] = 'l';
    block2[3] = 'l';
    block2[4] = 'o';
    block2[5] = ' ';
    block2[6] = 'W';
    dataPtr2.store(block2);
    BlockData new_block2 = dataPtr2.load();
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

TEST(DataPtrTest, DataPtrSave) {
  EXPECT_NO_THROW({
    FileManager fileManager("test_file.db");
    DataPtr dataPtr1 = fileManager.newPtr();
    DataPtr dataPtr2 = fileManager.newPtr();
    DataPtr dataPtr3 = fileManager.newPtr();
    BlockData block1 = dataPtr1.load();
    BlockData block2 = dataPtr2.load();
    BlockData block3 = dataPtr3.load();
    EXPECT_EQ(block1[0], 0);
    EXPECT_EQ(block2[0], 0);
    EXPECT_EQ(block3[0], 0);
    block2[0] = 'H';
    block2[1] = 'e';
    block2[2] = 'l';
    block2[3] = 'l';
    block2[4] = 'o';
    block2[5] = ' ';
    block2[6] = 'W';
    dataPtr2.store(block2);
    BlockData new_block2 = dataPtr2.load();
    EXPECT_EQ(new_block2[0], 'H');
    EXPECT_EQ(new_block2[1], 'e');
    EXPECT_EQ(new_block2[2], 'l');
    EXPECT_EQ(new_block2[3], 'l');
    EXPECT_EQ(new_block2[4], 'o');
    EXPECT_EQ(new_block2[5], ' ');
    EXPECT_EQ(fileManager.getPtrs().size(), 3);
    EXPECT_EQ(fileManager.getPtrs()[1].load()[0], 'H');
    
    Byte* bytes = dataPtr2.getBytes();
    DataPtr* dataPtr4 = DataPtr::fromBytes(bytes);

    BlockData new_block4 = dataPtr4->load();
    EXPECT_EQ(new_block4[0], 'H');
    EXPECT_EQ(new_block4[1], 'e');
    EXPECT_EQ(new_block4[2], 'l');
    EXPECT_EQ(new_block4[3], 'l');
    EXPECT_EQ(new_block4[4], 'o');
    EXPECT_EQ(new_block4[5], ' ');

    DataPtr dataPtr5(bytes);
    BlockData new_block5 = dataPtr5.load();
    EXPECT_EQ(new_block5[0], 'H');
    EXPECT_EQ(new_block5[1], 'e');
    EXPECT_EQ(new_block5[2], 'l');
    EXPECT_EQ(new_block5[3], 'l');
    EXPECT_EQ(new_block5[4], 'o');
    EXPECT_EQ(new_block5[5], ' ');

    remove("test_file.db");
  });
};
