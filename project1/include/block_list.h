#ifndef BLOCK_LIST_H
#define BLOCK_LIST_H

#include <fstream>
#include <memory>

#include "block_ptr.h"

class ListBlock {
 private:
  BlockPtr _block_ptr;
  mutable std::shared_ptr<ListBlock> _next_block;
  mutable bool _next_block_loaded;

  void save_header() const;
  void save_next_block_ptr(const BlockPtr &next_block_ptr) const;

 public:
  ListBlock(const BlockPtr &block_ptr, const BlockPtr &next_block_ptr);
  ListBlock(const BlockPtr &block_ptr);

  static std::size_t block_header_size();

  std::shared_ptr<ListBlock> next() const;
};

class BlockList {};

#endif  // BLOCK_LIST_H
