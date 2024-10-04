#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "block.h"
#include "block_ptr.h"
#include "data_ptr.h"
#include "file_manager.h"
#include "index.h"
#include "record.h"

class BPlusTreeNode {
 protected:
  BlockPtr _block_ptr;
  std::size_t n;
  IndexType _index_type;

 public:
  BPlusTreeNode(const IndexType index_type, const BlockPtr block_ptr,
                const int n = 0)
      : _block_ptr(block_ptr), n(n), _index_type(index_type) {}

  virtual void save_header() const;
  virtual void load_header();
  virtual std::size_t header_length() const;
  virtual void load() = 0;

  std::size_t data_length() const;

  friend class BPlusTree;
};

class BPlusTreeLeafNode : public BPlusTreeNode {
 private:
  std::shared_ptr<Schema> _schema;
  std::vector<Record> _records;
  std::vector<std::shared_ptr<Index>> _index;

 public:
  BPlusTreeLeafNode(const IndexType index_type, const bool create_new,
                    const BlockPtr &block_ptr,
                    const std::shared_ptr<Schema> &schema = nullptr,
                    const int n = 0);

  void load() override;
  void push_back(const Record &record);
};

class BPlusTreeInternalNode : public BPlusTreeNode {
 private:
  std::vector<BlockPtr> _son;
  std::vector<std::shared_ptr<Index>> _index;

 public:
  BPlusTreeInternalNode(const IndexType index_type, const bool create_new,
                        const BlockPtr &block_ptr, const int n = 0);

  void load() override;
};

class BPlusTree {
 private:
  std::shared_ptr<FileManager> _file_manager;
  std::shared_ptr<BPlusTreeInternalNode> _root;
  std::shared_ptr<Schema> _schema;

 public:
  BPlusTree(const std::string &file_name, const bool create_new,
            const std::shared_ptr<Schema> &schema);
};

#endif  // B_PLUS_TREE_H
