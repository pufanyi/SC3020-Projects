#include "block_ptr.h"

#include <fcntl.h>

#include <iostream>

BlockPtr::BlockPtr(const std::shared_ptr<std::fstream> &file,
                   const std::streamoff &offset,
                   const std::shared_ptr<BlockBuffer> &buffer)
    : _file(file), _offset(offset), _buffer(buffer) {}

BlockPtr::BlockPtr(const std::shared_ptr<std::fstream> &file, const Byte *bytes,
                   const std::shared_ptr<BlockBuffer> &buffer)
    : _file(file), _buffer(buffer) {
  memcpy(&_offset, bytes, sizeof(_offset));
}

std::shared_ptr<BlockData> BlockPtr::load_ptr() const {
  auto ptr = _buffer->from_buffer(_offset);

  if (ptr != nullptr) {
    return ptr;
  }

  std::shared_ptr<BlockData> block = std::make_shared<BlockData>();
  if (block == nullptr) {
    throw std::runtime_error("Error allocating memory");
  }
  _file->seekg(_offset, std::ios::beg);
  if (_file->fail()) {
    throw std::runtime_error("Error seeking to block");
  }
  _file->read(block->data, BLOCK_SIZE);
  if (_file->fail()) {
    throw std::runtime_error("Error reading block");
  }

  _buffer->to_buffer(_offset, block);
  return block;
}

BlockData &BlockPtr::load() const {
  std::shared_ptr<BlockData> block = load_ptr();
  return *block;
}

void BlockPtr::store(const BlockData &block) const {
  _buffer->update_buffer(_offset, block);
  _file->seekp(_offset, std::ios::beg);
  if (_file->fail()) {
    throw std::runtime_error("Error seeking to block");
  }
  _file->write(block.getData(), BLOCK_SIZE);
  if (_file->fail()) {
    throw std::runtime_error("Error writing block");
  }
  _file->flush();
}

Byte *BlockPtr::getBytes() const {
  Byte *bytes = new Byte[sizeof(_offset)];
  std::memcpy(bytes, &_offset, sizeof(_offset));
  return bytes;
}

std::size_t BlockPtr::size() { return sizeof(_offset); }
