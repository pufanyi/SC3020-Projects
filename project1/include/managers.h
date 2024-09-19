#ifndef MANAGERS_H
#define MANAGERS_H

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
              std::size_t max_blocks_cached = 100);

  ~FileManager();

  BlockPtr newPtr();
  BlockPtr getPtr(const std::streamoff &offset);
  BlockPtr getPtr(const Byte *bytes);

  const std::string &file_name() const { return _file_name; }
  std::size_t num_blocks() const { return _num_blocks; }

  std::vector<BlockPtr> getPtrs() const;
};

#endif  // MANAGERS_H
