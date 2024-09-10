#ifndef MANAGERS_H
#define MANAGERS_H

#include <fstream>
#include <vector>

#include "data_ptr.h"
#include "fields.h"

class FileManager {
  std::fstream file;
  size_t num_blocks;

 public:
  FileManager(const std::string &file_name);

  ~FileManager();

  DataPtr newPtr();
  std::vector<DataPtr> getPtrs();
};

#endif  // MANAGERS_H
