#include "data_ptr.h"

FileManager::FileManager(const std::string &file_name)
    : file(file_name, std::ios::in | std::ios::out | std::ios::binary) {
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
}

// DataPtr FileManager::newPtr() {
// }
