#include "buffer.h"

#include <iostream>

std::shared_ptr<BlockData> BlockBuffer::from_buffer(const BlockIndex &offset) {
  try {
    // std::cerr << offset << ' ' << _blocks.size() << std::endl;
    auto it = _blocks.find(offset);
    if (it != _blocks.end()) {
      return it->second;
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return nullptr;
}

void BlockBuffer::to_buffer(const BlockIndex &offset,
                            const std::shared_ptr<BlockData> &block) {
  if (_blocks.size() >= MAX_BLOCKS_CACHED) {
    // std::cerr << "Erase " <<  _blocks.size() << std::endl;
    BlockIndex offset = _block_queue.front();
    _block_queue.pop();
    auto ref_count_it = _block_ref_count.find(offset);
    if (ref_count_it->second > 1) {
      ref_count_it->second--;
    } else {
      _block_ref_count.erase(ref_count_it);
      auto it = _blocks.find(offset);
      if (it->second.use_count() > 1) {
        it->second->save();
      }
      _blocks.erase(it);
      // std::cerr << "Erase " << _blocks.size() << ' ' << offset << std::endl;
    }
  }

  _blocks[offset] = block;
  // std::cerr << "Insert " << _blocks.size() << std::endl;
  _block_queue.push(offset);
  _block_ref_count[offset]++;
}

void BlockBuffer::update_buffer(const BlockIndex &offset,
                                const BlockData &block) {
  std::cerr << "Update " << _blocks.size() << std::endl;
  auto it = _blocks.find(offset);
  if (it != _blocks.end()) {
    std::memcpy(it->second->data, block.data, BLOCK_SIZE);
  }
}
