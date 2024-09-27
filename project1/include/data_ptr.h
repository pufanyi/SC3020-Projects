#ifndef DATA_PTR_H
#define DATA_PTR_H

#include "block.h"
#include "block_ptr.h"
#include "fields.h"

class DataPtr {
 private:
  BlockPtr _block_ptr;
  off_t _offset;

  BlockData& getBlockData() const { return _block_ptr.load(); }

 public:
  DataPtr(BlockPtr& block_ptr, off_t offset)
      : _block_ptr(block_ptr), _offset(offset) {}

  DataPtr(const DataPtr& data_ptr) = default;

  virtual std::size_t size() = 0;

  void store(const Byte* bytes);
  const Byte* getBytes() const;

  Byte& operator[](std::size_t index);
  const Byte& operator[](std::size_t index) const;
};

#endif  // DATA_PTR_H
