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
  size_t size = fields.size();
  int begin = 0;
  for (auto &field : fields) {
    Byte *single_field = new Byte[field->getSize()];
    std::copy(bytes + begin, bytes + begin + field->getSize(), single_field);
    std::string value = field->bytesToString(single_field);
    result += value + " ";
    begin += field->getSize();
  }

  return result;
}

const Byte *Record::operator[](const std::string field_name) const {
  const Byte *bytes = this->getData();
  return bytes + _schema->dtypes()[field_name];
}

const Byte *Record::at(const std::string field_name) const {
  return this->operator[](field_name);
}

std::shared_ptr<Index> Record::getIndex(const IndexType &index_type,
                                        const std::string &field_name) const {
  if (!this->test()) {
    std::cerr << this->offset() << std::endl;
    throw std::runtime_error("Record test failed 1");
  }
  const Byte *bytes = this->operator[](field_name);
  if (!this->test()) {
    throw std::runtime_error("Record test failed 2");
  }
  std::shared_ptr<Index> index = createIndex(index_type);
  if (!this->test()) {
    throw std::runtime_error("Record test failed 3");
  }
  index->load(bytes);

  if (!this->test()) {
    throw std::runtime_error("Record test failed 4");
  }
  return index;
}
