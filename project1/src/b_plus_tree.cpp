#include "b_plus_tree.h"

#include <algorithm>
#include <cstddef>

void BPlusTreeNode::save_header() const {
  bool _is_leaf = is_leaf();
  _block_ptr.store(reinterpret_cast<const Byte *>(&_is_leaf), 0,
                   sizeof(_is_leaf));
  _block_ptr.store(reinterpret_cast<const Byte *>(&n), sizeof(_is_leaf),
                   sizeof(n) + sizeof(_is_leaf));
}

void BPlusTreeNode::load_header() {
  Byte *bytes = new Byte[sizeof(n)];
  _block_ptr.load(bytes, sizeof(bool), sizeof(n) + sizeof(bool));
  n = *reinterpret_cast<std::size_t *>(bytes);
  delete[] bytes;
}

std::size_t BPlusTreeNode::header_length() const {
  return sizeof(n) + sizeof(bool);
}

std::size_t BPlusTreeNode::data_length() const {
  return _block_ptr.size() - header_length();
}

BPlusTreeInternalNode::BPlusTreeInternalNode(const BPlusTree *b_plus_tree,
                                             const IndexType index_type,
                                             const bool create_new,
                                             const BlockPtr &block_ptr,
                                             const int n)
    : BPlusTreeNode(index_type, block_ptr, n), _b_plus_tree(b_plus_tree) {
  if (create_new) {
    save_header();
  } else {
    load();
  }
}

void BPlusTreeInternalNode::load() {
  load_header();
  BlockIndex now_offset = header_length();
  _index.clear();
  _son.clear();
  _index.reserve(n);
  _son.reserve(n + 1);
  for (decltype(n) i = 0; i <= 2 * n; ++i) {
    if (i % 2 == 0) {  // son
      BlockPtr son = _block_ptr.get_other(now_offset);
      std::shared_ptr<BPlusTreeNode> _now_son = _b_plus_tree->get_node(son);
      _son.push_back(_now_son);
      now_offset += sizeof(BlockIndex);
    } else {  // index
      std::shared_ptr<Index> index = createIndex(_index_type);
      Byte *bytes = new Byte[index->size()];
      _block_ptr.load(bytes, now_offset, now_offset + index->size());
      index->load(bytes);
      delete[] bytes;
      now_offset += index->size();
      _index.push_back(index);
    }
  }
}

BPlusTreeLeafNode::BPlusTreeLeafNode(const BPlusTree *b_plus_tree,
                                     const IndexType index_type,
                                     const bool create_new,
                                     const BlockPtr &block_ptr,
                                     const std::shared_ptr<Schema> &schema,
                                     const int n)
    : BPlusTreeNode(index_type, block_ptr, n),
      _schema(schema),
      _b_plus_tree(b_plus_tree) {
  if (create_new) {
    save_header();
  } else {
    load();
  }
}

void BPlusTreeLeafNode::load() {
  load_header();
  BlockIndex now_offset = header_length();
  _index.clear();
  _records.clear();
  _index.reserve(n);
  _records.reserve(n);
  for (decltype(n) i = 0; i < n; ++i) {
    // ptr
    BlockIndex ptr_offset;
    _block_ptr.load(reinterpret_cast<Byte *>(&ptr_offset), now_offset,
                    now_offset + sizeof(ptr_offset));
    BlockPtr _now_block_ptr = _block_ptr.get_other(ptr_offset);
    now_offset += sizeof(ptr_offset);

    BlockIndex record_offset;
    _block_ptr.load(reinterpret_cast<Byte *>(&record_offset), now_offset,
                    now_offset + sizeof(record_offset));
    _records.emplace_back(_now_block_ptr, record_offset, _schema);

    // index
    std::shared_ptr<Index> index = createIndex(_index_type);
    Byte *bytes = new Byte[index->size()];
    _block_ptr.load(bytes, now_offset, now_offset + index->size());
    index->load(bytes);
    delete[] bytes;
    now_offset += index->size();
  }
}

BlockIndex BPlusTreeLeafNode::now_offset() const {
  if (n == 0) {
    return header_length();
  } else {
    return header_length() + n * (sizeof(BlockIndex) * 2 + _index[0]->size());
  }
}

BlockIndex BPlusTreeInternalNode::now_offset() const {
  auto ans = header_length() + _son.size() * sizeof(BlockIndex);
  if (!_index.empty()) {
    ans += _index[0]->size() * _index.size();
  }
  return ans;
}

void BPlusTreeLeafNode::push_back(const Record &record,
                                  const std::shared_ptr<Index> &index) {
  _records.push_back(record);
  _index.push_back(index);
  Byte *bytes = new Byte[record.size()];
  record.store_ptr(bytes);
  _block_ptr.store(bytes, now_offset(), now_offset() + record.size());
  delete[] bytes;
  bytes = new Byte[index->size()];
  index->save(bytes);
  _block_ptr.store(bytes, now_offset() + record.size(),
                   now_offset() + record.size() + index->size());
  delete[] bytes;
  n++;
}

void BPlusTreeInternalNode::push_back(
    const std::shared_ptr<BPlusTreeNode> &son) {
  auto _now_offset = now_offset();
  _son.push_back(son);
  const BlockPtr &son_block_ptr = son->_block_ptr;
  Byte *bytes = new Byte[BlockPtr::size()];
  son_block_ptr.store_ptr(bytes);
  _block_ptr.store(bytes, _now_offset, _now_offset + BlockPtr::size());
  delete[] bytes;
}

void BPlusTreeInternalNode::push_back(const std::shared_ptr<Index> &index) {
  n++;
  auto _now_offset = now_offset();
  _index.push_back(index);
  Byte *bytes = new Byte[index->size()];
  index->save(bytes);
  _block_ptr.store(bytes, _now_offset, _now_offset + index->size());
  delete[] bytes;
}

BPlusTree::BPlusTree(const bool create_new, const IndexType index_type,
                     const std::string &index_name,
                     const std::string &index_file_name,
                     const std::shared_ptr<Schema> &schema)
    : _index_file_manager(
          std::make_shared<FileManager>(index_file_name, create_new)),
      // _data_file_manager(data_file_manager),
      _schema(schema),
      _root(nullptr),
      _index_name(index_name),
      _index_type(index_type) {
  if (create_new) {
    info_block_ptr = _index_file_manager->newPtr();
    auto root_ptr = _index_file_manager->newPtr();
    _root = std::make_shared<BPlusTreeLeafNode>(this, index_type, true,
                                                root_ptr, schema);
    _min_degree = calc_min_degree();
    save_info();
  } else {
    info_block_ptr = _index_file_manager->getFirstPtr();
    load_root();
  }
#if DEBUG
  std::cerr << "t = " << _min_degree << std::endl;
#endif
}

void BPlusTree::save_info() const {
  Byte *bytes = new Byte[BlockPtr::size() + sizeof(bool)];
  _root->_block_ptr.store_ptr(bytes);
  info_block_ptr.store(bytes, 0, BlockPtr::size());
  auto is_leaf = _root->is_leaf();
  info_block_ptr.store(reinterpret_cast<Byte *>(&is_leaf), BlockPtr::size(),
                       BlockPtr::size() + sizeof(is_leaf));
  delete[] bytes;
}

std::size_t BPlusTree::info_size() const {
  return BlockPtr::size() + sizeof(bool) + sizeof(std::size_t);
}

void BPlusTree::load_root() {
  Byte *bytes = new Byte[info_size()];
  info_block_ptr.load(bytes, 0, info_size());
  BlockPtr root_ptr = _index_file_manager->getPtr(bytes);
  bool is_leaf = *reinterpret_cast<bool *>(bytes + BlockPtr::size());
  if (is_leaf) {
    _root = std::make_shared<BPlusTreeLeafNode>(this, _index_type, false,
                                                root_ptr, _schema);
  } else {
    _root = std::make_shared<BPlusTreeInternalNode>(this, _index_type, false,
                                                    root_ptr);
  }
  memcpy(&_min_degree, bytes + BlockPtr::size() + sizeof(bool),
         sizeof(std::size_t));
  delete[] bytes;
}

std::size_t BPlusTree::calc_min_degree() const {
  auto index_size = getIndexSize(_index_type);
  auto record_size = 2 * sizeof(BlockIndex);
  auto block_size = sizeof(BlockIndex);
  auto remain_size = BLOCK_SIZE - sizeof(std::size_t);
  std::cerr << "Remain size: " << remain_size << std::endl;
  auto min_n = std::min((remain_size - block_size) / (index_size + record_size),
                        (remain_size - block_size) / (block_size + index_size));
  return min_n / 2;
}

BPlusTree::BPlusTree(const IndexType index_type, const std::string &index_name,
                     const std::string &index_file_name,
                     const std::shared_ptr<Schema> &schema,
                     const std::vector<Record> &records)
    : _index_file_manager(std::make_shared<FileManager>(index_file_name, true)),
      // _data_file_manager(data_file_manager),
      _schema(schema),
      _root(nullptr),
      _index_name(index_name),
      _index_type(index_type),
      _min_degree(calc_min_degree()) {
  info_block_ptr = _index_file_manager->newPtr();
  _root = bulk_load(records);
  save_info();
#if DEBUG
  std::cerr << "t = " << _min_degree << std::endl;
#endif
}

std::shared_ptr<BPlusTreeNode> BPlusTree::bulk_load(
    std::vector<Record> records) {
#if DEBUG
  std::cerr << "Bulk loading" << std::endl;
  std::cerr << "Number of records: " << records.size() << std::endl;
  for (const auto &record : records) {
    if (!record.test()) {
      throw std::runtime_error("Record test failed");
    }
  }
#endif

  std::vector<std::pair<std::shared_ptr<Index>, std::size_t>> indexes;
  for (decltype(records.size()) i = 0; i < records.size(); ++i) {
    indexes.emplace_back(records[i].getIndex(_index_type, _index_name), i);
  }

  std::sort(records.begin(), records.end(),
            [=](const Record &a, const Record &b) {
              return a.getIndex(_index_type, _index_name) <
                     b.getIndex(_index_type, _index_name);
            });

  std::vector<std::shared_ptr<BPlusTreeNode>> nodes;
  std::shared_ptr<BPlusTreeLeafNode> now_node = nullptr;
  std::size_t remain = records.size();
  std::shared_ptr<BPlusTreeLeafNode> last_node = nullptr;
  for (const auto &record : records) {
    if (now_node == nullptr) {
      auto block_ptr = _index_file_manager->newPtr();
      now_node = std::make_shared<BPlusTreeLeafNode>(this, _index_type, true,
                                                     block_ptr, _schema);
      if (last_node != nullptr) {
        last_node->set_next(now_node);
      }
      nodes.push_back(now_node);
    }
    now_node->push_back(record, record.getIndex(_index_type, _index_name));
    if (now_node->n == _min_degree && remain >= _min_degree) {
      last_node = now_node;
      now_node = nullptr;
    }
  }
  if (last_node != nullptr) {
    last_node->set_next(nullptr);
  }
  return bulk_load(nodes);
}

std::shared_ptr<BPlusTreeNode> BPlusTree::bulk_load(
    const std::vector<std::shared_ptr<BPlusTreeNode>> &nodes) {
  std::cerr << "Bulk loading" << std::endl;
  std::cerr << "Number of nodes: " << nodes.size() << std::endl;
  if (nodes.empty()) {
    return nullptr;
  }
  if (nodes.size() == 1) {
#if DEBUG
    std::cerr << "Root size: " << nodes[0]->n << std::endl;
#endif
    return nodes[0];
  }
  std::vector<std::shared_ptr<BPlusTreeNode>> new_nodes;
  std::shared_ptr<BPlusTreeInternalNode> now_node = nullptr;
  std::size_t remain = nodes.size();
  for (const auto &node : nodes) {
    if (now_node == nullptr) {
      auto block_ptr = _index_file_manager->newPtr();
      now_node = std::make_shared<BPlusTreeInternalNode>(this, _index_type,
                                                         true, block_ptr);
      now_node->push_back(node);
    } else {
      now_node->push_back(node->min_index());
      now_node->push_back(node);
    }
    // #if DEBUG
    // std::cerr << "Now node size: " << now_node->n << std::endl;
    // #endif
    if (now_node->n == _min_degree && remain >= _min_degree) {
      new_nodes.push_back(now_node);
      now_node = nullptr;
    }
  }
  if (now_node != nullptr) {
    new_nodes.push_back(now_node);
  }
  return bulk_load(new_nodes);
}

void BPlusTreeLeafNode::range_query(const std::shared_ptr<Index> &begin,
                                    const std::shared_ptr<Index> &end,
                                    std::vector<Record> &result) const {
  for (decltype(n) i = 0; i < n; ++i) {
    if (*_index[i] >= *begin) {
      if (*_index[i] > *end) {
        return;
      }
      result.push_back(_records[i]);
    }
  }
  if (_next != nullptr) {
    _next->range_query(begin, end, result);
  }
}

void BPlusTreeInternalNode::range_query(const std::shared_ptr<Index> &begin,
                                        const std::shared_ptr<Index> &end,
                                        std::vector<Record> &result) const {
  auto it = std::lower_bound(_index.begin(), _index.end(), begin,
                             [=](const std::shared_ptr<Index> &a,
                                 const std::shared_ptr<Index> &b) {
                               return *a < *b;
                             });
  if (it == _index.end()) {
    _son.back()->range_query(begin, end, result);
  } else {
    auto son_it = _son.begin() + (it - _index.begin());
    (*son_it)->range_query(begin, end, result);
  }
}

std::shared_ptr<BPlusTreeNode> BPlusTree::get_node(
    const BlockPtr &block_ptr) const {
  Byte *bytes = new Byte[sizeof(bool)];
  block_ptr.load(bytes, 0, sizeof(bool));
  bool is_leaf = *reinterpret_cast<bool *>(bytes);
  delete[] bytes;
  if (is_leaf) {
    return std::make_shared<BPlusTreeLeafNode>(this, _index_type, false,
                                               block_ptr, _schema);
  } else {
    return std::make_shared<BPlusTreeInternalNode>(this, _index_type, false,
                                                   block_ptr);
  }
}

void BPlusTreeLeafNode::set_next(
    const std::shared_ptr<BPlusTreeLeafNode> &next) {
  _next = next;
  Byte *bytes = new Byte[BlockPtr::size()];
  if (next == nullptr) {
    memset(bytes, 0, BlockPtr::size());
  } else {
    next->_block_ptr.store_ptr(bytes);
  }
  _block_ptr.store(bytes, BLOCK_SIZE - BlockPtr::size(), BLOCK_SIZE);
}

std::shared_ptr<BPlusTreeLeafNode> BPlusTreeLeafNode::next() const {
  return _next;
}

void BPlusTreeLeafNode::load_next() {
  Byte *bytes = new Byte[BlockPtr::size()];
  _block_ptr.load(bytes, BLOCK_SIZE - BlockPtr::size(), BLOCK_SIZE);
  BlockIndex next_offset;
  memcpy(&next_offset, bytes, sizeof(BlockIndex));
  if (next_offset == 0) {
    _next = nullptr;
  } else {
    _next = std::make_shared<BPlusTreeLeafNode>(
        _b_plus_tree, _index_type, false, _block_ptr.get_other(next_offset),
        _schema);
  }
}
