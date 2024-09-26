#include "buffer.h"

std::shared_ptr<BlockData> BlockBuffer::from_buffer(
    const std::streamoff &offset) {
  auto it = _blocks.find(offset);
  if (it != _blocks.end()) {
    return it->second;
  }
  return nullptr;
}

void BlockBuffer::to_buffer(const std::streamoff &offset,
                            const std::shared_ptr<BlockData> &block) {
  if (_blocks.size() >= MAX_BLOCKS_CACHED) {
    std::streamoff offset = _block_queue.front();
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
    }
  }

  _blocks[offset] = block;
  _block_queue.push(offset);
  _block_ref_count[offset]++;
}

void BlockBuffer::update_buffer(const std::streamoff &offset,
                                const BlockData &block) {
  auto it = _blocks.find(offset);
  if (it != _blocks.end()) {
    std::memcpy(it->second->data, block.data, BLOCK_SIZE);
  }
}
