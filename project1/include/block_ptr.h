#ifndef BLOCK_PTR_H
#define BLOCK_PTR_H

#include <fstream>
#include <memory>

#include "block.h"
#include "buffer.h"
#include "utils.h"

class BlockPtr {
 private:
  BlockIndex _offset;
  std::shared_ptr<std::fstream> _file;
  std::shared_ptr<BlockBuffer> _buffer;
  mutable std::weak_ptr<BlockData> _block_data;

 public:
  BlockPtr(const std::shared_ptr<std::fstream> &file, const BlockIndex &offset,
           const std::shared_ptr<BlockBuffer> &buffer);
  BlockPtr(const std::shared_ptr<std::fstream> &file, const Byte *bytes,
           const std::shared_ptr<BlockBuffer> &buffer);
  BlockPtr(const BlockPtr &block_ptr) = default;

  BlockPtr get_other(const Byte *bytes) const {
    return BlockPtr(_file, bytes, _buffer);
  }
  BlockPtr get_other(const BlockIndex &offset) const {
    return BlockPtr(_file, offset, _buffer);
  }

  std::size_t offset() const { return _offset; }

  std::weak_ptr<BlockData> load_ptr() const;
  BlockData &load() const;
  void load(Byte *dst, const std::size_t begin, const std::size_t end) const;
  void store(const BlockData &block) const;
  void store(const Byte *bytes, std::size_t begin, std::size_t end) const;
  Byte *getBytes() const;
  static std::size_t size();
};

#endif  // BLOCK_PTR_H
