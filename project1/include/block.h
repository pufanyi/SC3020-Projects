#ifndef BLOCK_H
#define BLOCK_H

#include <memory>
#include <cstring>

#include "utils.h"

class Block {
private:
  char *data;

 public:
  Block();
  Block(const char* data);
  Block(const Block &block);

  virtual ~Block() { delete[] data; }

  const char &operator[](std::size_t index) const;
  char &operator[](std::size_t index);
};

#endif  // BLOCK_H
