#include "fields.h"

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::shared_ptr<Field> FieldCreator::createField(const FieldType& type,
                                                 const std::size_t size) {
  if (size == 0) {
    throw std::invalid_argument("Invalid field size");
  }
  if (size > 1 && type != FieldType::VARCHAR) {
    throw std::invalid_argument(
        "Size greater than 1 is only allowed for VARCHAR fields");
  }
  switch (type) {
    case FieldType::INT:
      return std::make_shared<IntField>();
    case FieldType::CHAR:
      return std::make_shared<CharField>();
    case FieldType::DATE:
      return std::make_shared<DateField>();
    case FieldType::FLOAT32:
      return std::make_shared<Float32Field>();
    case FieldType::FLOAT64:
      return std::make_shared<Float64Field>();
    case FieldType::BOOLEAN:
      return std::make_shared<BooleanField>();
    case FieldType::VARCHAR:
      return std::make_shared<VarcharField>(size);
    default:
      throw std::invalid_argument("Invalid field type");
  }
}

std::shared_ptr<Field> FieldCreator::createField(std::string type) {
  std::transform(type.begin(), type.end(), type.begin(), ::toupper);
  FieldType fieldType;
  std::size_t size = 1;

  if (type == "INT") {
    fieldType = FieldType::INT;
  } else if (type == "CHAR") {
    fieldType = FieldType::CHAR;
  } else if (type == "DATE") {
    fieldType = FieldType::DATE;
  } else if (type == "FLOAT32") {
    fieldType = FieldType::FLOAT32;
  } else if (type == "FLOAT64") {
    fieldType = FieldType::FLOAT64;
  } else if (type == "BOOLEAN") {
    fieldType = FieldType::BOOLEAN;
  } else if (type.substr(0, 7) == "VARCHAR") {
    fieldType = FieldType::VARCHAR;
    const auto start = type.find("(");
    const auto end = type.find(")");
    if (start == std::string::npos || end == std::string::npos ||
        start >= end - 1) {
      throw std::invalid_argument("Invalid VARCHAR field format");
    }
    std::string size_str = type.substr(start + 1, end - start - 1);
    std::stringstream ss(size_str);
    if (!(ss >> size) || size == 0 || !ss.eof()) {
      throw std::invalid_argument("Invalid VARCHAR field size");
    }
  } else {
    throw std::invalid_argument("Invalid field type");
  }

  return createField(fieldType, size);
}

Byte* IntField::stringToBytes(const std::string& value) const {
  // Parse the field
  try {
    int int_value = std::stoi(value);
    static char bytes[sizeof(int_value)];
    std::copy(reinterpret_cast<const char*>(
                  reinterpret_cast<const void*>(&int_value)),
              reinterpret_cast<const char*>(
                  reinterpret_cast<const void*>(&int_value)) +
                  sizeof(int_value),
              bytes);
    return bytes;
  } catch (const std::invalid_argument& e) {
    throw std::runtime_error("Invalid integer value");
  } catch (const std::out_of_range& e) {
    throw std::runtime_error("Integer value out of range");
  }
}

std::string IntField::bytesToString(const Byte* value) const {
  int int_value;
  std::memcpy(&int_value, value, sizeof(int));
  return std::to_string(int_value);
}

Byte* CharField::stringToBytes(const std::string& value) const {
  Byte* byte_value = reinterpret_cast<Byte*>(const_cast<char*>(value.c_str()));
  return byte_value;
}

std::string CharField::bytesToString(const Byte* value) const {
  return std::string(reinterpret_cast<const char*>(value));
}

Byte* DateField::stringToBytes(const std::string& value) const {
  if (value.length() != 10 || value[2] != '/' || value[5] != '/') {
    throw std::invalid_argument("Invalid date format");
  }
  int d, m, y;
  if (sscanf(value.c_str(), "%d/%d/%d", &d, &m, &y) != 3) {
    throw std::invalid_argument("Failed to parse date");
  }
  if (d < 1 || d > 31 || m < 1 || m > 12 || y < 0 || y > 9999) {
    throw std::invalid_argument("Invalid date values");
  }
  Byte* byte_value = reinterpret_cast<Byte*>(const_cast<char*>(value.c_str()));

  return byte_value;
}

std::string DateField::bytesToString(const Byte* value) const {
  return std::string(reinterpret_cast<const char*>(value));
}

Byte* BooleanField::stringToBytes(const std::string& value) const {
  if (value == "1") {
    Byte* byte_value = new Byte[1];
    byte_value[0] = 1;
    return byte_value;
  } else if (value == "0") {
    Byte* byte_value = new Byte[1];
    byte_value[0] = 0;
    return byte_value;
  } else {
    throw std::invalid_argument("Invalid boolean value");
  }
}

std::string BooleanField::bytesToString(const Byte* value) const {
  if (value[0] == 1) {
    return "1";
  } else if (value[0] == 0) {
    return "0";
  } else {
    throw std::invalid_argument("Invalid boolean value");
  }
}

Byte* Float32Field::stringToBytes(const std::string& value) const {
  try {
    float float_value = std::stof(value);
    static char bytes[sizeof(float_value)];
    std::copy(reinterpret_cast<const char*>(
                  reinterpret_cast<const void*>(&float_value)),
              reinterpret_cast<const char*>(
                  reinterpret_cast<const void*>(&float_value)) +
                  sizeof(float_value),
              bytes);
    return bytes;
  } catch (const std::invalid_argument& e) {
    throw std::runtime_error("Invalid float value");
  } catch (const std::out_of_range& e) {
    throw std::runtime_error("Float value out of range");
  }
}
Byte* Float64Field::stringToBytes(const std::string& value) const {
  try {
    double float_value = std::stod(value);
    static char bytes[sizeof(float_value)];
    std::copy(reinterpret_cast<const char*>(
                  reinterpret_cast<const void*>(&float_value)),
              reinterpret_cast<const char*>(
                  reinterpret_cast<const void*>(&float_value)) +
                  sizeof(float_value),
              bytes);
    return bytes;
  } catch (const std::invalid_argument& e) {
    throw std::runtime_error("Invalid float value");
  } catch (const std::out_of_range& e) {
    throw std::runtime_error("Float value out of range");
  }
}

std::string Float32Field::bytesToString(const Byte* value) const {
  try {
    float result;
    std::copy(reinterpret_cast<const char*>(&value[0]),
              reinterpret_cast<const char*>(&value[4]),
              reinterpret_cast<char*>(&result));
    return std::to_string(result);
  } catch (const std::invalid_argument& e) {
    throw std::runtime_error("Invalid float value");
  } catch (const std::out_of_range& e) {
    throw std::runtime_error("Float value out of range");
  }
}
std::string Float64Field::bytesToString(const Byte* value) const {
  try {
    double result;
    std::copy(reinterpret_cast<const char*>(&value[0]),
              reinterpret_cast<const char*>(&value[8]),
              reinterpret_cast<char*>(&result));
    return std::to_string(result);
  } catch (const std::invalid_argument& e) {
    throw std::runtime_error("Invalid float value");
  } catch (const std::out_of_range& e) {
    throw std::runtime_error("Double value out of range");
  }
}

Byte* VarcharField::stringToBytes(const std::string& value) const {
  if (value.length() > size) {
    throw std::invalid_argument("String value too long");
  }
  Byte* byte_value = new Byte[size];
  std::fill(byte_value, byte_value + size, 0);
  std::copy(value.begin(), value.end(), byte_value);
  return byte_value;
}

std::string VarcharField::bytesToString(const Byte* value) const {
  std::string result;
  for (std::size_t i = 0; i < size; i++) {
    if (value[i] == 0) {
      break;
    }
    result.push_back(value[i]);
  }
  return result;
}

void DataTypes::addField(const std::string& field_name,
                         const std::shared_ptr<Field>& field) {
  field_names.push_back(field_name);
  fields.push_back(field);
}

void DataTypes::addField(const std::string& field_name, const FieldType type,
                         const std::size_t size) {
  field_names.push_back(field_name);
  auto field = FieldCreator::createField(type, size);
  fields.push_back(field);
}

void DataTypes::addField(const std::string& field_name,
                         const std::string& type) {
  field_names.push_back(field_name);
  auto field = FieldCreator::createField(type);
  fields.push_back(field);
}

DataTypes::Iterator DataTypes::begin() const {
  return Iterator(field_names.begin(), fields.begin());
}

DataTypes::Iterator DataTypes::end() const {
  return Iterator(field_names.end(), fields.end());
}

std::pair<const std::string&, const std::shared_ptr<Field>&>
DataTypes::Iterator::operator*() const {
  return {*name_it, *field_it};
}

DataTypes::Iterator& DataTypes::Iterator::operator++() {
  ++name_it;
  ++field_it;
  return *this;
}

bool DataTypes::Iterator::operator!=(const Iterator& other) const {
  return name_it != other.name_it || field_it != other.field_it;
}

bool DataTypes::Iterator::operator==(const Iterator& other) const {
  return name_it == other.name_it && field_it == other.field_it;
}

std::ostream& operator<<(std::ostream& os, const Field& field) {
  os << field.to_string();
  return os;
}

std::string DataTypes::to_string() const {
  std::stringstream ss;
  for (const auto& [name, field] : *this) {
    ss << name << " " << *field << ",\n";
  }
  std::string result = ss.str();
  return result.substr(0, result.size() - 2);
}

std::string DataTypes::to_table(const std::size_t min_name_length,
                                const std::size_t min_type_length) const {
  std::stringstream ss;

  auto max_name_length = min_name_length - 2;
  auto max_type_length = min_type_length - 2;

  // Determine maximum lengths
  for (const auto& [name, field] : *this) {
    max_name_length = std::max(max_name_length, name.length());
    max_type_length = std::max(max_type_length, field->to_string().length());
  }

  // Add some padding
  max_name_length += 2;
  max_type_length += 2;

  // Create the header
  ss << "+" << std::string(max_name_length, '-') << "+"
     << std::string(max_type_length, '-') << "+\n";
  ss << "| " << std::left << std::setw(max_name_length - 2) << "Field Name"
     << " | " << std::setw(max_type_length - 2) << "Field Type"
     << " |\n";
  ss << "+" << std::string(max_name_length, '-') << "+"
     << std::string(max_type_length, '-') << "+\n";

  // Add rows
  for (const auto& [name, field] : *this) {
    ss << "| " << std::left << std::setw(max_name_length - 2) << name << " | "
       << std::setw(max_type_length - 2) << field->to_string() << " |\n";
  }

  // Add footer
  ss << "+" << std::string(max_name_length, '-') << "+"
     << std::string(max_type_length, '-') << "+\n";

  return ss.str();
}

std::ostream& operator<<(std::ostream& os, const DataTypes& data_types) {
  os << data_types.to_table();
  return os;
}

std::size_t DataTypes::size() const {
  std::size_t size = 0;
  for (const auto& [name, field] : *this) {
    size += field->getSize();
  }
  return size;
}
