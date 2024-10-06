#include "data_ptr.h"

#include "file_manager.h"

void DataPtr::store(const Byte* bytes) {
  _block_ptr.store(bytes, _offset, _offset + size());
}

void DataPtr::store(const Byte* bytes, std::size_t begin, std::size_t end) {
  _block_ptr.store(bytes, _offset + begin, _offset + end);
}

void DataPtr::load(Byte* bytes) const {
  _block_ptr.load(bytes, _offset, _offset + size());
}

void DataPtr::load(Byte* bytes, std::size_t begin, std::size_t end) const {
  _block_ptr.load(bytes, _offset + begin, _offset + end);
}

const Byte* DataPtr::getBytes() const { return &getBlockData()[_offset]; }

Byte& DataPtr::operator[](std::size_t index) {
  return getBlockData()[_offset + index];
}

const Byte& DataPtr::operator[](std::size_t index) const {
  return getBlockData()[_offset + index];
}

void DataPtr::store_ptr(Byte* bytes) const {
  _block_ptr.store_ptr(bytes);
  // memcpy(bytes + _block_ptr.size(), &_offset, sizeof(BlockIndex));
  std::copy(reinterpret_cast<const Byte*>(&_offset),
            reinterpret_cast<const Byte*>(&_offset) + sizeof(BlockIndex),
            bytes + _block_ptr.size());
}

void DataPtr::load_ptr(std::shared_ptr<FileManager> file_manager, Byte* bytes) {
  BlockIndex block_offset;
  // memcpy(&block_offset, bytes, sizeof(BlockIndex));
  std::copy(bytes, bytes + sizeof(BlockIndex),
            reinterpret_cast<Byte*>(&block_offset));
  _block_ptr = file_manager->getPtr(block_offset);
  // memcpy(&_offset, bytes + sizeof(BlockIndex), sizeof(off_t));
  std::copy(bytes + sizeof(BlockIndex),
            bytes + sizeof(BlockIndex) + sizeof(off_t),
            reinterpret_cast<Byte*>(&_offset));
}
