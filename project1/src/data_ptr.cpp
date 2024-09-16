#include "data_ptr.h"

DataPtr::DataPtr(std::fstream *file, const std::streamoff &offset)
    : file(file), offset(offset) {}

DataPtr::DataPtr(const Byte* bytes) {
  std::memcpy(&file, bytes, sizeof(std::fstream *));
  std::memcpy(&offset, bytes + sizeof(std::fstream *), sizeof(std::streamoff));
}

BlockData DataPtr::load() const {
  BlockData block;

  // Seek to the position of the block
  file->seekg(offset, std::ios::beg);

  // Read the block from the file
  file->read(reinterpret_cast<char *>(block.data), BLOCK_SIZE);

  // Return the block
  return block;
}

void DataPtr::store(const BlockData &block) const {
  // Seek to the position of the block
  file->seekp(offset, std::ios::beg);

  // Write the block to the file
  file->write(reinterpret_cast<const char *>(block.data), BLOCK_SIZE);

  // Flush to ensure the block is written
  file->flush();
}

Byte* DataPtr::getBytes() const {
  Byte *bytes = new Byte[sizeof(std::fstream *) + sizeof(std::streamoff)];
  std::memcpy(bytes, &file, sizeof(std::fstream *));
  std::memcpy(bytes + sizeof(std::fstream *), &offset, sizeof(std::streamoff));
  return bytes;
}

size_t DataPtr::size() { return sizeof(std::fstream *) + sizeof(std::streamoff); }

DataPtr* DataPtr::fromBytes(const Byte* bytes) {
  return new DataPtr(bytes);
}
