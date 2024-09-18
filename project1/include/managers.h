#ifndef MANAGERS_H
#define MANAGERS_H

#include <fstream>
#include <vector>

#include "block_ptr.h"
#include "fields.h"

class FileManager {
  std::shared_ptr<std::fstream> file;
  const std::string _file_name;
  std::size_t _num_blocks;

 public:
  FileManager(const std::string &file_name, bool create_new = false);

  ~FileManager();

  BlockPtr newPtr();
  BlockPtr getPtr(const std::streamoff &offset) {
    return BlockPtr(file, offset);
  }
  BlockPtr getPtr(const Byte *bytes) { return BlockPtr(file, bytes); }

  const std::string &file_name() const { return _file_name; }
  std::size_t num_blocks() const { return _num_blocks; }

  std::vector<BlockPtr> getPtrs() const {
    std::vector<BlockPtr> ptrs;
    for (std::size_t i = 0; i < _num_blocks; i++) {
      ptrs.push_back(BlockPtr(file, i * BLOCK_SIZE));
    }
    return ptrs;
  }
};

#endif  // MANAGERS_H
