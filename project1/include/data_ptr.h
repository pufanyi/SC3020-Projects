#ifndef DATA_PTR_H
#define DATA_PTR_H

#include "block.h"
#include "block_ptr.h"
#include "fields.h"

class DataPtr {
 private:
  BlockPtr _block_ptr;
  off_t _offset;

 protected:
  BlockData& getBlockData() const { return _block_ptr.load(); }

  std::shared_ptr<std::fstream> file() const { return _block_ptr.file(); }

  std::shared_ptr<BlockBuffer> buffer() const { return _block_ptr.buffer(); }

  BlockIndex block_id() const { return _block_ptr.offset(); }

  const BlockPtr& block() const { return _block_ptr; }

  off_t offset() const { return _offset; }

 public:
  DataPtr(const BlockPtr& block_ptr, off_t offset)
      : _block_ptr(block_ptr), _offset(offset) {}

  DataPtr(const DataPtr& data_ptr) = default;

  virtual std::size_t size() const = 0;

  void store(const Byte* bytes);
  void store(const Byte* bytes, std::size_t begin, std::size_t end);
  void load(Byte* bytes) const;
  void load(Byte* bytes, std::size_t begin, std::size_t end) const;
  const Byte* getBytes() const;

  Byte& operator[](std::size_t index);
  const Byte& operator[](std::size_t index) const;
};

#endif  // DATA_PTR_H
