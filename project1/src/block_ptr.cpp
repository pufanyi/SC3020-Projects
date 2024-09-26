#include "block_ptr.h"

#include <fcntl.h>

#include <iostream>

BlockPtr::BlockPtr(const std::shared_ptr<std::fstream> &file,
                   const std::streamoff &offset,
                   const std::shared_ptr<BlockBuffer> &buffer)
    : _file(file), _offset(offset), _buffer(buffer) {
  this->load_ptr();
}

BlockPtr::BlockPtr(const std::shared_ptr<std::fstream> &file, const Byte *bytes,
                   const std::shared_ptr<BlockBuffer> &buffer)
    : _file(file), _buffer(buffer) {
  memcpy(&_offset, bytes, sizeof(_offset));
  this->load_ptr();
}

std::weak_ptr<BlockData> BlockPtr::load_ptr() const {
  if (!_block_data.expired()) {
    return _block_data;
  }

  auto ptr = _buffer->from_buffer(_offset);

  if (ptr != nullptr) {
    return ptr;
  }

  std::shared_ptr<BlockData> block =
      std::make_shared<BlockData>(_offset, _file);
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

  return _block_data = block;
}

BlockData &BlockPtr::load() const {
  std::weak_ptr<BlockData> block = load_ptr();
  if (block.expired()) {
    throw std::runtime_error("Error loading block");
  }
  return *block.lock();
}

void BlockPtr::store(const BlockData &block) const {
  auto &block_data = load();
  memcpy(block_data.data, block.data, BLOCK_SIZE);
}

Byte *BlockPtr::getBytes() const {
  Byte *bytes = new Byte[sizeof(_offset)];
  std::memcpy(bytes, &_offset, sizeof(_offset));
  return bytes;
}

std::size_t BlockPtr::size() { return sizeof(_offset); }
