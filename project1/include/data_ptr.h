#ifndef DATA_PTR_H
#define DATA_PTR_H

#include "block.h"
#include "block_ptr.h"
#include "fields.h"
#include "file_manager.h"

class DataPtr {
 private:
  BlockPtr _block_ptr;
  off_t _offset;

 protected:
  BlockData& getBlockData() const { return _block_ptr.load(); }

  std::shared_ptr<std::fstream> file() const { return _block_ptr.file(); }

  std::shared_ptr<BlockBuffer> buffer() const { return _block_ptr.buffer(); }

  const BlockPtr& block() const { return _block_ptr; }

 public:
  off_t offset() const { return _offset; }

  BlockIndex block_id() const { return _block_ptr.offset(); }

#if DEBUG
  bool test() const { return _block_ptr.buffer() != nullptr; }
#endif

  DataPtr(const BlockPtr& block_ptr, off_t offset)
      : _block_ptr(block_ptr), _offset(offset) {}

  DataPtr(const DataPtr& data_ptr) = default;

  virtual std::size_t size() const = 0;

  void store(const Byte* bytes);
  void store(const Byte* bytes, std::size_t begin, std::size_t end);
  void load(Byte* bytes) const;
  void load(Byte* bytes, std::size_t begin, std::size_t end) const;
  const Byte* getBytes() const;

  Byte& operator[](std::size_t index);
  const Byte& operator[](std::size_t index) const;

  virtual ~DataPtr() = default;

  void store_ptr(Byte* bytes) const;
  void load_ptr(std::shared_ptr<FileManager> file_manager, Byte* bytes);
};

#endif  // DATA_PTR_H
