#include "block.h"

BlockData::BlockData() : data(new Byte[BLOCK_SIZE]) {
  memset(data, 0, BLOCK_SIZE);
}

BlockData::BlockData(const Byte *data) : data(new Byte[BLOCK_SIZE]) {
  memset(this->data, 0, BLOCK_SIZE);
  if (data != nullptr) {
    memcpy(this->data, data, BLOCK_SIZE * sizeof(Byte));
  }
}

BlockData::BlockData(const BlockData &block) : data(new Byte[BLOCK_SIZE]) {
  memcpy(data, block.data, BLOCK_SIZE);
}

const Byte &BlockData::operator[](std::size_t index) const {
  return data[index];
}

Byte &BlockData::operator[](std::size_t index) { return data[index]; }
