#include "managers.h"

#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

FileManager::FileManager(const std::string &file_name, bool create_new,
                         std::size_t max_blocks_cached)
    : _file_name(file_name),
      buffer(std::make_shared<BlockBuffer>(max_blocks_cached)) {
  std::ios_base::openmode mode =
      std::ios::in | std::ios::out | std::ios::binary;
  if (create_new) {
    mode |= std::ios::trunc;
  }

  file = std::make_shared<std::fstream>(file_name, mode);
  if (!file->is_open()) {
    throw std::runtime_error("Error opening file");
  }

  file->seekg(0, std::ios::end);
  std::streampos fileSize = file->tellg();
  if (fileSize == -1) {
    throw std::runtime_error("Error seeking file");
  }

  if (create_new) {
    _num_blocks = 0;
  } else {
    if (fileSize % BLOCK_SIZE != 0) {
      std::stringstream ss;
      ss << "File size is not a multiple of block size, file size: " << fileSize
         << ", block size: " << BLOCK_SIZE;
      throw std::runtime_error(ss.str());
    }
    _num_blocks = fileSize / BLOCK_SIZE;
  }
}

FileManager::~FileManager() { file->close(); }

BlockPtr FileManager::newPtr() {
  file->seekp(0, std::ios::end);
  std::streampos fileEnd = file->tellp();
  if (fileEnd == -1) {
    throw std::runtime_error("Error seeking to end of file");
  }

  Byte newBlock[BLOCK_SIZE];

  file->write(newBlock, BLOCK_SIZE);
  if (file->fail()) {
    throw std::runtime_error("Error writing new block to file");
  }

  file->flush();
  if (file->fail()) {
    throw std::runtime_error("Error flushing file");
  }

  this->_num_blocks++;

  return BlockPtr(file, fileEnd, buffer);
}

std::vector<BlockPtr> FileManager::getPtrs() const {
  std::vector<BlockPtr> ptrs;
  for (std::size_t i = 0; i < _num_blocks; i++) {
    ptrs.push_back(BlockPtr(file, i * BLOCK_SIZE, buffer));
  }
  return ptrs;
}

BlockPtr FileManager::getPtr(const std::streamoff &offset) {
  return BlockPtr(file, offset, buffer);
}
BlockPtr FileManager::getPtr(const Byte *bytes) {
  return BlockPtr(file, bytes, buffer);
}
