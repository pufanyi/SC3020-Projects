#include "b_plus_tree.h"

#include <cstddef>

void BPlusTreeNode::save_header() const {
  _block_ptr.store(reinterpret_cast<const Byte*>(&n), 0, sizeof(n));
}

void BPlusTreeNode::load_header() {
  Byte* bytes = new Byte[sizeof(n)];
  _block_ptr.load(bytes, 0, sizeof(n));
  n = *reinterpret_cast<std::size_t*>(bytes);
  delete[] bytes;
}

std::size_t BPlusTreeNode::header_length() const { return sizeof(n); }

std::size_t BPlusTreeNode::data_length() const {
  return _block_ptr.size() - header_length();
}
