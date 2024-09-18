#ifndef BUFFER_H
#define BUFFER_H

#include <queue>
#include <memory>
#include <unordered_map>

#include "block.h"

class BlockBuffer {
  private:
  std::unordered_map<std::streamoff, std::shared_ptr<BlockData>> _blocks;
  std::unordered_map<std::streamoff, std::size_t> _block_ref_count;
  std::queue<std::streamoff> _block_queue;

  public:
  const std::size_t MAX_BLOCKS_CACHED;

  BlockBuffer(std::size_t max_blocks_cached = 100)
      : MAX_BLOCKS_CACHED(max_blocks_cached) {}
  
  std::shared_ptr<BlockData> from_buffer(const std::streamoff &offset);
  void to_buffer(const std::streamoff &offset, const std::shared_ptr<BlockData> &block);
  void update_buffer(const std::streamoff &offset, const BlockData &block);
};

#endif  // BUFFER_H
