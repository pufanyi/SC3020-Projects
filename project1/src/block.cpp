#include "block.h"

Block::Block() : data(new char[BLOCK_SIZE]) {}

Block::Block(const char *data) : data(new char[BLOCK_SIZE]) {
  if (data != nullptr) {
    memcpy(this->data, data, BLOCK_SIZE);
  } else {
    memset(this->data, 0, BLOCK_SIZE);
  }
}

Block::Block(const Block &block) : data(new char[BLOCK_SIZE]) {
  memcpy(data, block.data, BLOCK_SIZE);
}

const char &Block::operator[](std::size_t index) const { return data[index]; }

char &Block::operator[](std::size_t index) { return data[index]; }
