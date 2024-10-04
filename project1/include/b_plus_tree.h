#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "block.h"
#include "block_ptr.h"

class BPlusTreeNode {
 protected:
  BlockPtr _block_ptr;

  std::size_t n;

 public:
};

class BPlusTreeLeafNode : public BPlusTreeNode {};

class BPlusTreeInternalNode : public BPlusTreeNode {};

#endif  // B_PLUS_TREE_H
