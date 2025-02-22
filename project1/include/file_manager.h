#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include <vector>

#include "block_ptr.h"
#include "buffer.h"
#include "fields.h"

class FileManager {
  std::shared_ptr<std::fstream> file;
  const std::string _file_name;
  std::size_t _num_blocks;
  std::shared_ptr<BlockBuffer> buffer;

 public:
  FileManager(const std::string &file_name, bool create_new = false,
              std::size_t max_blocks_cached = 500);

  BlockPtr newPtr();
  BlockPtr getPtr(const BlockIndex &offset);
  BlockPtr getPtr(const Byte *bytes);
  BlockPtr getFirstPtr();

  const std::string &file_name() const { return _file_name; }
  std::size_t num_blocks() const { return _num_blocks; }

  std::vector<BlockPtr> getPtrs() const;
};

#endif  // FILE_MANAGER_H
