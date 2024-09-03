#ifndef BLOCK_H
#define BLOCK_H

#include <cstring>
#include <memory>

#include "utils.h"

class BlockData {
 private:
  Byte *data;

 public:
  BlockData();
  BlockData(const Byte *data);
  BlockData(const BlockData &block);

  virtual ~BlockData() { delete[] data; }

  const Byte &operator[](std::size_t index) const;
  Byte &operator[](std::size_t index);

  const Byte *getData() const;

  friend class DataPtr;
};

#endif  // BLOCK_H
