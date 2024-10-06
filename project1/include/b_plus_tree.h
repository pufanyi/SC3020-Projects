#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "block.h"
#include "block_ptr.h"
#include "data_ptr.h"
#include "file_manager.h"
#include "index.h"
#include "record.h"

class BPlusTree;

class BPlusTreeNode {
 protected:
  BlockPtr _block_ptr;
  std::size_t n;  // number of index
  IndexType _index_type;

 public:
  BPlusTreeNode(const IndexType index_type, const BlockPtr block_ptr,
                const int n = 0)
      : _block_ptr(block_ptr), n(n), _index_type(index_type) {}

  virtual void save_header() const;
  virtual void load_header();
  virtual std::size_t header_length() const;
  virtual void load() = 0;

  virtual bool is_leaf() const = 0;

  virtual std::shared_ptr<Index> min_index() const = 0;
  virtual std::shared_ptr<Index> max_index() const = 0;

  std::size_t data_length() const;

  virtual void range_query(const std::shared_ptr<Index> &begin,
                           const std::shared_ptr<Index> &end,
                           std::vector<Record> &result) const = 0;

  friend class BPlusTree;
  friend class BPlusTreeLeafNode;
  friend class BPlusTreeInternalNode;
};

class BPlusTreeLeafNode : public BPlusTreeNode {
 private:
  std::shared_ptr<Schema> _schema;
  std::vector<Record> _records;
  std::vector<std::shared_ptr<Index>> _index;
  const BPlusTree *_b_plus_tree;
  std::shared_ptr<BPlusTreeLeafNode> _next;

 public:
  BPlusTreeLeafNode(const BPlusTree *b_plus_tree, const IndexType index_type,
                    const bool create_new, const BlockPtr &block_ptr,
                    const std::shared_ptr<Schema> &schema = nullptr,
                    const int n = 0);

  void set_next(const std::shared_ptr<BPlusTreeLeafNode> &next);
  std::shared_ptr<BPlusTreeLeafNode> next() const;
  void load_next();

  BlockIndex now_offset() const;

  void load() override;
  void push_back(const Record &record, const std::shared_ptr<Index> &index);

  bool is_leaf() const override { return true; }

  std::shared_ptr<Index> min_index() const override {
    if (_index.empty()) {
      return nullptr;
    }
    return _index.front();
  }

  std::shared_ptr<Index> max_index() const override {
    if (_index.empty()) {
      return nullptr;
    }
    return _index.back();
  }

  void range_query(const std::shared_ptr<Index> &begin,
                   const std::shared_ptr<Index> &end,
                   std::vector<Record> &result) const override;
};

class BPlusTreeInternalNode : public BPlusTreeNode {
 private:
  std::vector<BlockPtr> _son;
  std::vector<std::shared_ptr<Index>> _index;
  const BPlusTree *_b_plus_tree;

 protected:
  void push_back(const std::shared_ptr<BPlusTreeNode> &son);
  void push_back(const std::shared_ptr<Index> &index);

 public:
  BPlusTreeInternalNode(const BPlusTree *b_plus_tree,
                        const IndexType index_type, const bool create_new,
                        const BlockPtr &block_ptr, const int n = 0);

  void load() override;

  BlockIndex now_offset() const;

  bool is_leaf() const override { return false; }

  std::shared_ptr<Index> min_index() const override {
    if (_index.empty()) {
      return nullptr;
    }
    return _index.front();
  }

  std::shared_ptr<Index> max_index() const override {
    if (_index.empty()) {
      return nullptr;
    }
    return _index.back();
  }

  void range_query(const std::shared_ptr<Index> &begin,
                   const std::shared_ptr<Index> &end,
                   std::vector<Record> &result) const override;

  friend class BPlusTree;
};

class BPlusTree {
 private:
  std::shared_ptr<FileManager> _index_file_manager;
  std::shared_ptr<BPlusTreeNode> _root;
  std::shared_ptr<Schema> _schema;
  BlockPtr info_block_ptr;
  const std::string _index_name;
  const IndexType _index_type;
  std::size_t _min_degree;

  std::size_t calc_min_degree() const;

  std::shared_ptr<BPlusTreeNode> bulk_load(std::vector<Record> records);
  std::shared_ptr<BPlusTreeNode> bulk_load(
      const std::vector<std::shared_ptr<BPlusTreeNode>> &nodes);

 public:
  void save_info() const;
  void load_root();

  std::size_t info_size() const;

  BPlusTree(const IndexType index_type, const std::string &index_name,
            const std::string &index_file_name,
            const std::shared_ptr<Schema> &schema,
            const std::vector<Record> &records);  // bulk loading

  BPlusTree(const bool create_new, const IndexType index_type,
            const std::string &index_name, const std::string &index_file_name,
            const std::shared_ptr<Schema> &schema);

  std::vector<Record> range_query(const std::shared_ptr<Index> &begin,
                                  const std::shared_ptr<Index> &end) const;

  std::shared_ptr<BPlusTreeNode> get_node(const BlockPtr &block_ptr) const;
};

#endif  // B_PLUS_TREE_H
