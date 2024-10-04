#include "b_plus_tree.h"

#include <cstddef>

void BPlusTreeNode::save_header() const {
  _block_ptr.store(reinterpret_cast<const Byte *>(&n), 0, sizeof(n));
}

void BPlusTreeNode::load_header() {
  Byte *bytes = new Byte[sizeof(n)];
  _block_ptr.load(bytes, 0, sizeof(n));
  n = *reinterpret_cast<std::size_t *>(bytes);
  delete[] bytes;
}

std::size_t BPlusTreeNode::header_length() const { return sizeof(n); }

std::size_t BPlusTreeNode::data_length() const {
  return _block_ptr.size() - header_length();
}

BPlusTreeInternalNode::BPlusTreeInternalNode(const IndexType index_type,
                                             const bool create_new,
                                             const BlockPtr &block_ptr,
                                             const int n)
    : BPlusTreeNode(index_type, block_ptr, n) {
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
      _son.push_back(son);
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

BPlusTreeLeafNode::BPlusTreeLeafNode(const IndexType index_type,
                                     const bool create_new,
                                     const BlockPtr &block_ptr,
                                     const std::shared_ptr<Schema> &schema,
                                     const int n)
    : BPlusTreeNode(index_type, block_ptr, n), _schema(schema) {
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

void BPlusTreeInternalNode::push_back(const BPlusTreeNode &son) {
  auto _now_offset = now_offset();
  _son.push_back(son._block_ptr);
  const BlockPtr &son_block_ptr = son._block_ptr;
  Byte *bytes = new Byte[BlockPtr::size()];
  son_block_ptr.store_ptr(bytes);
  _block_ptr.store(bytes, _now_offset, _now_offset + BlockPtr::size());
  delete[] bytes;
}

void BPlusTreeInternalNode::push_back(const std::shared_ptr<Index> &index) {
  auto _now_offset = now_offset();
  _index.push_back(index);
  Byte *bytes = new Byte[index->size()];
  index->save(bytes);
  _block_ptr.store(bytes, _now_offset, _now_offset + index->size());
  delete[] bytes;
}
