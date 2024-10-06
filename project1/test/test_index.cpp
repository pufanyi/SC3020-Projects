#include <gtest/gtest.h>

#include <memory>

#include "index.h"

TEST(IndexTest, IntIndex) {
  IntIndex index(123);
  Byte bytes[sizeof(int)];
  index.save(bytes);
  IntIndex new_index;
  new_index.load(bytes);
  EXPECT_EQ(index, new_index);
}

TEST(IndexTest, Float32Index) {
  Float32Index index(123.33);
  Byte bytes[sizeof(float)];
  index.save(bytes);
  std::shared_ptr<Index> new_index = createIndex(IndexType::FLOAT32);
  new_index->load(bytes);
  EXPECT_EQ(index, *new_index);
}
