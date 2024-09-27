#include "block_list.h"

#include "utils.h"

ListBlock::ListBlock(const BlockPtr &block_ptr, const BlockPtr &next_block_ptr)
    : _block_ptr(block_ptr), _next_block(nullptr), _next_block_loaded(false) {
  save_header();
}

ListBlock::ListBlock(const BlockPtr &block_ptr)
    : _block_ptr(block_ptr), _next_block(nullptr), _next_block_loaded(false) {}

std::size_t ListBlock::block_header_size() { return sizeof(BlockIndex); }

void ListBlock::save_next_block_ptr(const BlockPtr &next_block_ptr) const {
  auto _block_ptr_offset = _block_ptr.offset();
  next_block_ptr.store(reinterpret_cast<Byte *>(&_block_ptr_offset),
                       BLOCK_SIZE - sizeof(BlockIndex), BLOCK_SIZE);
}

void ListBlock::save_header() const {
  Byte bytes[block_header_size()];
  auto _block_ptr_offset = _block_ptr.offset();
  _block_ptr.store(bytes, 0, block_header_size());
}

std::shared_ptr<ListBlock> ListBlock::next() const {
  if (!_next_block_loaded) {
    BlockIndex next_block_offset;
    _block_ptr.load(reinterpret_cast<Byte *>(&next_block_offset),
                    BLOCK_SIZE - sizeof(BlockIndex), BLOCK_SIZE);
    _next_block_loaded = true;
    _next_block =
        std::make_shared<ListBlock>(_block_ptr.get_other(next_block_offset));
  }
  return _next_block;
}
