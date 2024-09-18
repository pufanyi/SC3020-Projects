#include "block_ptr.h"

#include <fcntl.h>

#include <iostream>

std::unordered_map<std::streamoff, std::shared_ptr<BlockData>>
    BlockPtr::_blocks;
std::unordered_map<std::streamoff, std::size_t> BlockPtr::_block_ref_count;
std::queue<std::streamoff> BlockPtr::_block_queue;

BlockPtr::BlockPtr(const std::shared_ptr<std::fstream> &file,
                   const std::streamoff &offset)
    : _file(file), _offset(offset) {}

BlockPtr::BlockPtr(const std::shared_ptr<std::fstream> &file, const Byte *bytes)
    : _file(file) {
  memcpy(&_offset, bytes, sizeof(_offset));
}

std::shared_ptr<BlockData> BlockPtr::load_ptr() const {
  auto it = _blocks.find(_offset);
  if (it != _blocks.end()) {
    return it->second;
  }

  if (_blocks.size() >= MAX_BLOCKS_CACHED) {
    std::streamoff offset = _block_queue.front();
    _block_queue.pop();
    auto ref_count_it = _block_ref_count.find(offset);
    if (ref_count_it->second > 1) {
      ref_count_it->second--;
    } else {
      _block_ref_count.erase(ref_count_it);
      _blocks.erase(offset);
    }
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

  _block_queue.push(_offset);
  _block_ref_count[_offset]++;
  return _blocks[_offset] = block;
}

BlockData &BlockPtr::load() const {
  std::shared_ptr<BlockData> block = load_ptr();
  return *block;
}

void BlockPtr::store(const BlockData &block) const {
  auto it = _blocks.find(_offset);
  if (it != _blocks.end()) {
    *it->second = block;
  }
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
