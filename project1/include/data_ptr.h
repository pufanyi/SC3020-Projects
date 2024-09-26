#ifndef DATA_PTR_H
#define DATA_PTR_H

#include "block.h"
#include "block_ptr.h"
#include "fields.h"

class DataPtr {
 private:
  std::shared_ptr<Field> _field;
  BlockPtr _block_ptr;
  off_t _offset;

  BlockData& getBlockData() const { return _block_ptr.load(); }

 public:
  DataPtr(const std::shared_ptr<Field>& field,
          BlockPtr& block_ptr, off_t offset)
      : _field(field), _block_ptr(block_ptr), _offset(offset) {}

  DataPtr(const FieldType& type, const BlockPtr& block_ptr,
          off_t offset)
      : _field(FieldCreator::createField(type)),
        _block_ptr(block_ptr),
        _offset(offset) {}

  DataPtr(const DataPtr& data_ptr) = default;

  void store(const std::string& value);
  void store(const Byte* bytes);

  template <typename T>
  void store_value(const T& value) {
    Byte* bytes = _field->valueToBytes(value);
    store(bytes);
    delete[] bytes;
  }

  Byte* getBytes() const;

  std::string load_str();

  template <typename T>
  void load(T& value) {
    _field->bytesToValue(&_block_ptr.load()[_offset], value);
  }

  template <typename T>
  void load_value(T& value) {
    _field->bytesToValue(&_block_ptr.load()[_offset], value);
  }
};

#endif  // DATA_PTR_H
