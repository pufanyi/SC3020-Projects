#include "data_ptr.h"

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
