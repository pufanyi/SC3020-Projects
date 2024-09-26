#ifndef DATA_PTR_H
#define DATA_PTR_H

#include "block.h"
#include "fields.h"

class DataPtr {
 private:
  std::shared_ptr<Field> _field;
  std::shared_ptr<BlockData> _block_data;
  std::size_t _offset;

 public:
  DataPtr(const std::shared_ptr<Field>& field,
          const std::shared_ptr<BlockData>& block_data, std::size_t offset)
      : _field(field), _block_data(block_data), _offset(offset) {}

  DataPtr(const FieldType& type, const std::shared_ptr<BlockData>& block_data,
          std::size_t offset)
      : _field(FieldCreator::createField(type)),
        _block_data(block_data),
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
    _field->bytesToValue(&(*_block_data)[_offset], value);
  }

  template <typename T>
  void load_value(T& value) {
    _field->bytesToValue(&(*_block_data)[_offset], value);
  }
};

#endif  // DATA_PTR_H
