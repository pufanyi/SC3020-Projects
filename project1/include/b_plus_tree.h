#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "block.h"
#include "block_ptr.h"
#include "data_ptr.h"
#include "index.h"

class BPlusTreeNode {
 protected:
  BlockPtr _block_ptr;
  std::size_t n;

 public:
  BPlusTreeNode(const BlockPtr& block_ptr) : _block_ptr(block_ptr), n(0) {
    load_header();
  }

  virtual void save_header() const;
  virtual void load_header();
  virtual std::size_t header_length() const;

  std::size_t data_length() const;
};

class BPlusTreeLeafNode : public BPlusTreeNode {
 private:
  std::vector<std::shared_ptr<DataPtr>> _ptr;
  std::vector<Index> _index;
};

class BPlusTreeInternalNode : public BPlusTreeNode {
 private:
  std::vector<std::shared_ptr<BlockPtr>> _son;
  std::vector<Index> _index;
};

class BPlusTreeRootNode : public BPlusTreeNode {
 private:
  std::vector<std::shared_ptr<BlockPtr>> _son;
  std::vector<Index> _index;
};

class BPlusTree {
 private:
};

#endif  // B_PLUS_TREE_H
