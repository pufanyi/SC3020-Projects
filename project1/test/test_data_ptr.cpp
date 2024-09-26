#include <gtest/gtest.h>

#include "data_ptr.h"
#include "fields.h"
#include "managers.h"

TEST(DataPtrTest, DataPtrConstructor) {
  EXPECT_NO_THROW({
    FileManager fileManager("test_file.db", true);
    BlockPtr block_ptr = fileManager.newPtr();
    off_t offset = 10;
    FieldType type = FieldType::FLOAT64;
    double x = 233.33;
    DataPtr data_ptr(type, block_ptr, offset);
    data_ptr.store_value(x);
    double y;
    data_ptr.load(y);
    EXPECT_DOUBLE_EQ(x, y);
  });
}
