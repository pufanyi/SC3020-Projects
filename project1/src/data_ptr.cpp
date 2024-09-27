#include "data_ptr.h"

void DataPtr::store(const Byte* bytes) {
  memcpy(&getBlockData()[_offset], bytes, size());
}

const Byte* DataPtr::getBytes() const { return &getBlockData()[_offset]; }

Byte& DataPtr::operator[](std::size_t index) {
  return getBlockData()[_offset + index];
}

const Byte& DataPtr::operator[](std::size_t index) const {
  return getBlockData()[_offset + index];
}
