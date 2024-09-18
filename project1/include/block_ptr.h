#ifndef BLOCK_PTR_H
#define BLOCK_PTR_H

#include <fstream>
#include <memory>

#include "buffer.h"
#include "block.h"
#include "utils.h"

class BlockPtr {
 private:
  std::streamoff _offset;
  std::shared_ptr<std::fstream> _file;
  std::shared_ptr<BlockBuffer> _buffer;

 public:
  BlockPtr(const std::shared_ptr<std::fstream> &file,
           const std::streamoff &offset, const std::shared_ptr<BlockBuffer> &buffer);
  BlockPtr(const std::shared_ptr<std::fstream> &file, const Byte *bytes, const std::shared_ptr<BlockBuffer> &buffer);

  BlockPtr get_other(const Byte *bytes) const { return BlockPtr(_file, bytes, _buffer); }
  BlockPtr get_other(const std::streamoff &offset) const {
    return BlockPtr(_file, offset, _buffer);
  }

  std::size_t offset() const { return _offset; }

  std::shared_ptr<BlockData> load_ptr() const;
  BlockData &load() const;
  void store(const BlockData &block) const;
  Byte *getBytes() const;
  static std::size_t size();
};

#endif  // BLOCK_PTR_H
