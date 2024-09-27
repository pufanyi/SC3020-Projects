#include "data_ptr.h"

void DataPtr::store(const Byte* bytes) {
  memcpy(&_block_ptr.load()[_offset], bytes, size());
}

const Byte* DataPtr::getBytes() const { return &_block_ptr.load()[_offset]; }

Byte& DataPtr::operator[](std::size_t index) {
  return _block_ptr.load()[_offset + index];
}

const Byte& DataPtr::operator[](std::size_t index) const {
  return _block_ptr.load()[_offset + index];
}
