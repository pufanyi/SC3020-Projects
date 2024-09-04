#include "managers.h"

FileManager::FileManager(const std::string &file_name)
    : file(file_name, std::ios::in | std::ios::out | std::ios::binary) {
  if (!file.is_open()) {
    file.open(file_name, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to create file");
    }
    file.close();

    // Reopen the file in read/write mode
    file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file after creating it");
    }
  }
}

FileManager::~FileManager() { file.close(); }

DataPtr FileManager::newPtr() {
  // Get the current position at the end of the file
  file.seekp(0, std::ios::end);
  std::streampos fileEnd = file.tellp();

  // Create a new block of zeros
  char newBlock[BLOCK_SIZE];
  // Instead of variable-sized assignment,
  // use this
  memset(newBlock, 0, BLOCK_SIZE);

  // Write the new block to the end of the file
  file.write(newBlock, BLOCK_SIZE);

  // Flush to ensure the block is written
  file.flush();

  // Create and return a DataPtr object pointing to the new block
  return DataPtr(&file, static_cast<std::streamoff>(
                            fileEnd));  // Record the position of the new block
}
