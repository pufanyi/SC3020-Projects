#ifndef DATA_PTR_H
#define DATA_PTR_H

#include <fstream>

#include "utils.h"

class DataPtr {
};

class FileManager {
  std::fstream file;

 public:
  FileManager(const std::string &file_name);

  ~FileManager() { file.close(); }

  DataPtr newPtr();
};

#endif  // DATA_PTR_H
