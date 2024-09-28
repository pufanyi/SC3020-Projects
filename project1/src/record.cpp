#include "record.h"

#include <iomanip>

constexpr std::size_t MIN_TABLE_WIDTH = 6;

std::ostream &operator<<(std::ostream &os, const Record &record) {
  std::size_t ptr = 0;
  const Byte *bytes = record.getBytes();
  std::vector<std::string> names;
  std::vector<std::string> data;
  std::vector<std::size_t> sizes;
  for (const auto [name, field] : record._schema->dtypes()) {
    names.push_back(name);
    std::string value = field->bytesToString(bytes + ptr);
    data.push_back(value);
    sizes.push_back(
        std::max(MIN_TABLE_WIDTH, std::max(name.size(), value.size())));
    ptr += field->getSize();
  }
  os << "|";
  for (decltype(names.size()) i = 0; i < names.size(); i++) {
    os << " " << std::setw(sizes[i]) << names[i] << " |";
  }
  os << "\n|";
  for (decltype(data.size()) i = 0; i < data.size(); i++) {
    os << " " << std::setw(sizes[i]) << data[i] << " |";
  }
  return os;
}
void Record::RecordPtr::save(Byte *bytes) const {
  BlockIndex _block_offset = _block.offset();
  memcpy(bytes, &_block_offset, sizeof(BlockIndex));
  memcpy(bytes + sizeof(BlockIndex), &_pos, sizeof(BlockIndex));
}

void Record::RecordPtr::load(const Byte *bytes,
                             const std::shared_ptr<std::fstream> &file,
                             const std::shared_ptr<BlockBuffer> &buffer,
                             const std::shared_ptr<Schema> &schema) {
  BlockIndex _block_offset;
  memcpy(&_block_offset, bytes, sizeof(BlockIndex));
  _block = BlockPtr(file, _block_offset, buffer);
  memcpy(&_pos, bytes + sizeof(std::size_t), sizeof(std::size_t));
}

bool Record::RecordPtr::is_nullptr() const { return _block.is_nullptr(); }

std::shared_ptr<Record> Record::next() const {
  if (_next == nullptr) {
    Byte *bytes = new Byte[record_header_size()];
    load(bytes, sizeof(BlockIndex), record_header_size());
    _next = std::make_shared<RecordPtr>(bytes, file(), buffer(), _schema);
    delete[] bytes;
  }

  return std::make_shared<Record>(_next->block(), _next->pos(), _schema);
}

void Record::set_next(const std::shared_ptr<Record> &next) {
  Byte *bytes = new Byte[RecordPtr::size()];
  _next = std::make_shared<RecordPtr>(next->block(), next->offset());
  _next->save(bytes);
  store(bytes, sizeof(BlockIndex), record_header_size());
  delete[] bytes;
}
