#include "fields.h"

#include <cstring>
#include <iostream>
#include <string>

Field* FieldCreator::createField(FieldType& type, size_t size) {
  if (size == 0) {
    throw std::invalid_argument("Invalid field size");
  }
  if (size > 1 && type != FieldType::VARCHAR) {
    throw std::invalid_argument(
        "Size greater than 1 is only allowed for VARCHAR fields");
  }
  switch (type) {
    case FieldType::INT:
      return new IntField();
    case FieldType::CHAR:
      return new CharField();
    case FieldType::DATE:
      return new DateField();
    case FieldType::FLOAT:
      return new FloatField();
    case FieldType::BOOLEAN:
      return new BooleanField();
    case FieldType::VARCHAR:
      return new VarcharField(size);
    default:
      throw std::invalid_argument("Invalid field type");
  }
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

Byte* FloatField::stringToBytes(const std::string& value) const {
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

std::string FloatField::bytesToString(const Byte* value) const {
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

Byte* VarcharField::stringToBytes(const std::string& value) const {
  if (value.length() > size) {
    throw std::invalid_argument("String value too long");
  }
  Byte* byte_value = reinterpret_cast<Byte*>(const_cast<char*>(value.c_str()));
  return byte_value;
}

std::string VarcharField::bytesToString(const Byte* value) const {
  return std::string(reinterpret_cast<const char*>(value));
}
