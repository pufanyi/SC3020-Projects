#ifndef MANAGERS_H
#define MANAGERS_H

#include <fstream>

#include "data_ptr.h"

class FileManager {
  std::fstream file;

 public:
  FileManager(const std::string &file_name);

  ~FileManager();

  DataPtr newPtr();
};

#endif  // MANAGERS_H
