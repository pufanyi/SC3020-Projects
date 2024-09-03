#ifndef DATA_PTR_H
#define DATA_PTR_H

#include <fstream>

#include "block.h"
#include "utils.h"

class DataPtr {
 private:
  std::fstream *file;
  std::streamoff offset;

 public:
  DataPtr(std::fstream *file, const std::streamoff &offset);

  BlockData load() const;
  void store(const BlockData &block) const;
};

class FileManager {
  std::fstream file;

 public:
  FileManager(const std::string &file_name);

  ~FileManager();

  DataPtr newPtr();
};

#endif  // DATA_PTR_H
