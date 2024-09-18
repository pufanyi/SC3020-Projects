#ifndef BLOCK_PTR_H
#define BLOCK_PTR_H

#include <fstream>
#include <memory>
#include <queue>
#include <unordered_map>

#include "block.h"
#include "utils.h"

class BlockPtr {
 private:
  std::streamoff _offset;
  std::shared_ptr<std::fstream> _file;

  static std::unordered_map<std::streamoff, std::shared_ptr<BlockData>> _blocks;
  static std::unordered_map<std::streamoff, std::size_t> _block_ref_count;
  static std::queue<std::streamoff> _block_queue;
  static const std::size_t MAX_BLOCKS_CACHED = 100;

 public:
  BlockPtr(const std::shared_ptr<std::fstream> &file,
           const std::streamoff &offset);
  BlockPtr(const std::shared_ptr<std::fstream> &file, const Byte *bytes);

  BlockPtr get_other(const Byte *bytes) const { return BlockPtr(_file, bytes); }
  BlockPtr get_other(const std::streamoff &offset) const {
    return BlockPtr(_file, offset);
  }

  std::size_t offset() const { return _offset; }

  std::shared_ptr<BlockData> load_ptr() const;
  BlockData &load() const;
  void store(const BlockData &block) const;
  Byte *getBytes() const;
  static std::size_t size();
};

#endif  // BLOCK_PTR_H
