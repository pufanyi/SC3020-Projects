#ifndef BUFFER_H
#define BUFFER_H

#include <memory>
#include <queue>
#include <unordered_map>

#include "block.h"

class BlockBuffer {
 private:
  std::unordered_map<BlockIndex, std::shared_ptr<BlockData>> _blocks;
  std::unordered_map<BlockIndex, std::size_t> _block_ref_count;
  std::queue<BlockIndex> _block_queue;

 public:
  const std::size_t MAX_BLOCKS_CACHED;

  BlockBuffer(std::size_t max_blocks_cached = 100)
      : MAX_BLOCKS_CACHED(max_blocks_cached) {}

  std::shared_ptr<BlockData> from_buffer(const BlockIndex &offset);
  void to_buffer(const BlockIndex &offset,
                 const std::shared_ptr<BlockData> &block);
  void update_buffer(const BlockIndex &offset, const BlockData &block);
};

#endif  // BUFFER_H
