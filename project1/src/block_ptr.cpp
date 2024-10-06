#include "block_ptr.h"

#include <fcntl.h>

#include <iostream>

BlockPtr::BlockPtr(const std::shared_ptr<std::fstream> &file,
                   const BlockIndex &offset,
                   const std::shared_ptr<BlockBuffer> &buffer)
    : _file(file), _offset(offset), _buffer(buffer) {
  this->load_ptr();
}

BlockPtr::BlockPtr(const std::shared_ptr<std::fstream> &file, const Byte *bytes,
                   const std::shared_ptr<BlockBuffer> &buffer)
    : _file(file), _buffer(buffer) {
  // memcpy(&_offset, bytes, sizeof(_offset));
  std::copy(bytes, bytes + sizeof(_offset), reinterpret_cast<Byte *>(&_offset));
  this->load_ptr();
}

std::weak_ptr<BlockData> BlockPtr::load_ptr() const {
  LOAD_PTR_TIMES++;
  if (!_block_data.expired()) {
    return _block_data;
  }

  if (_buffer == nullptr) {
    throw std::runtime_error("Error loading block, buffer is null");
  }

  if (_buffer == nullptr || _file == nullptr) {
    throw std::runtime_error("Error loading block, buffer is null");
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
  // Reading block from disk
  IO_TIMES++;

  return _block_data = block;
}

BlockData &BlockPtr::load() const {
  std::weak_ptr<BlockData> block = load_ptr();
  if (block.expired()) {
    throw std::runtime_error("Error loading block");
  }
  return *block.lock();
}

void BlockPtr::load(Byte *dst, const std::size_t begin,
                    const std::size_t end) const {
  if (end > BLOCK_SIZE || begin > end) {
    throw std::runtime_error("Error reading block");
  }
  auto &block_data = load();
  // memcpy(dst, block_data.data + begin, end - begin);
  std::copy(block_data.data + begin, block_data.data + end, dst);
}

void BlockPtr::store(const BlockData &block) const {
  auto &block_data = load();
  // memcpy(block_data.data, block.data, BLOCK_SIZE);
  std::copy(block.data, block.data + BLOCK_SIZE, block_data.data);
}

Byte *BlockPtr::getBytes() const {
  auto &block_data = load();
  return block_data.data;
}

void BlockPtr::store(const Byte *bytes, std::size_t begin,
                     std::size_t end) const {
  if (end > BLOCK_SIZE || begin > end) {
    throw std::runtime_error("Error writing block");
  }
  auto &block_data = load();
  // memcpy(block_data.data + begin, bytes, end - begin);
  std::copy(bytes, bytes + end - begin, block_data.data + begin);
}

void BlockPtr::store_ptr(Byte *bytes) const {
  // std::cerr << "========== Store ptr ==========" << std::endl;
  // for (int i = 0; i < sizeof(_offset); i++) {
  //   std::cerr << (int)bytes[i] << ' ';
  // }
  // std::cerr << std::endl;
  // std::memcpy(bytes, &_offset, sizeof(_offset));
  std::copy(reinterpret_cast<const Byte *>(&_offset),
            reinterpret_cast<const Byte *>(&_offset) + sizeof(_offset), bytes);
  // for (int i = 0; i < sizeof(_offset); i++) {
  //   std::cerr << (int)bytes[i] << ' ';
  // }
  // std::cerr << std::endl;
  // std::cerr << "========== End Store ==========" << std::endl;
}

std::size_t BlockPtr::size() { return sizeof(_offset); }

bool BlockPtr::is_nullptr() const { return _file == nullptr; }
