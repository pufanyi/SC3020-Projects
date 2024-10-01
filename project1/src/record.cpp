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

std::size_t Record::size() const { return _schema->row_size(); }

const Byte *Record::getData() const { return getBytes(); }

std::string Record::toString() const {
  std::vector<std::shared_ptr<Field>> fields = _schema->dtypes().getFields();
  const Byte *bytes = this->getData();
  std::string result = "";
  int index = 0;
  size_t size = fields.size();
  std::cout << "Size: " << size << std::endl;
  std::cout << "Bytes: " << bytes << std::endl;
  // for (const auto &field : fields) {
  // Byte *single_field = std::copy(bytes + index, bytes + index +
  // field->getSize(), single_field); result +=
  // field->bytesToString(single_field) + " ";
  // }
  return result;
}
