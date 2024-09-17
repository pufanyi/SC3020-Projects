#ifndef FIELD_H
#define FIELD_H

#include <cstring>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "utils.h"

enum class FieldType {
  INT,
  CHAR,
  VARCHAR,
  DATE,
  FLOAT,
  BOOLEAN,
};

class Field {
 protected:
  FieldType type;
  size_t size;

 public:
  Field() = default;
  Field(const FieldType type, const size_t size) : type(type), size(size) {}

  virtual ~Field() = default;
  virtual Byte* stringToBytes(const std::string& value) const = 0;
  virtual std::string bytesToString(const Byte* value) const = 0;

  template <typename T>
  Byte* valueToBytes(const T& value) const {
    std::stringstream ss;
    ss << value;
    std::string str_value = ss.str();
    return stringToBytes(str_value);
  }

  template <typename T>
  void bytesToValue(const Byte* value, T& result) const {
    std::string str_value = bytesToString(value);
    std::stringstream ss(str_value);
    ss >> result;
  }
};

class IntField : public Field {
 public:
  IntField() : Field(FieldType::INT, sizeof(int)) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
};

class CharField : public Field {
 public:
  CharField() : Field(FieldType::CHAR, sizeof(char)) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
};

class DateField : public Field {
 public:
  DateField() : Field(FieldType::DATE, strlen("1999/99/99") * sizeof(char)) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
};

class FloatField : public Field {
 public:
  FloatField() : Field(FieldType::FLOAT, sizeof(float)) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
};

class BooleanField : public Field {
 public:
  BooleanField() : Field(FieldType::BOOLEAN, sizeof(bool)) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
};

class VarcharField : public Field {
 private:
  const size_t size;

 public:
  VarcharField(const size_t size)
      : Field(FieldType::VARCHAR, size * sizeof(char)), size(size) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
  const size_t getSize() const { return size; }
};

class FieldCreator {
 public:
  static std::shared_ptr<Field> createField(const FieldType& type,
                                            const size_t size = 1);

  // VARCHAR should be "VARCHAR(size)"
  static std::shared_ptr<Field> createField(std::string type);
};

class DataTypes {
 private:
  std::vector<std::string> field_names;
  std::vector<std::shared_ptr<Field>> fields;

 public:
  DataTypes() = default;
  DataTypes(std::vector<std::shared_ptr<Field>> fields)
      : fields(std::move(fields)) {}
  virtual ~DataTypes() = default;

  void addField(const std::string& field_name,
                const std::shared_ptr<Field>& field);
  void addField(const std::string& field_name, const FieldType type,
                const size_t size = 1);
  void addField(const std::string& field_name, const std::string& type);
};

#endif
