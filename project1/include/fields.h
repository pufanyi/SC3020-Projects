#ifndef FIELD_H
#define FIELD_H

#include <cassert>
#include <cstring>
#include <iostream>
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
  FLOAT32,
  FLOAT64,
  BOOLEAN,
};

class Field {
 protected:
  FieldType type;
  std::size_t size;

 public:
  Field() = default;
  Field(const FieldType type, const std::size_t size)
      : type(type), size(size) {}

  virtual ~Field() = default;
  virtual Byte* stringToBytes(const std::string& value) const = 0;
  virtual std::string bytesToString(const Byte* value) const = 0;
  virtual std::string to_string() const = 0;

  const FieldType getType() const { return type; }
  const std::size_t getSize() const { return size; }

  template <typename T>
  Byte* valueToBytes(const T& value) const {
    Byte* data = nullptr;
    if constexpr (std::is_same_v<T, std::string>) {
      data = stringToBytes(value);
    } else {
      data = new Byte[size];
      assert(size == sizeof(T));
      memcpy(data, &value, size);
    }
    return data;
  }

  template <typename T>
  void bytesToValue(const Byte* value, T& result) const {
    if constexpr (std::is_same_v<T, std::string>) {
      result = bytesToString(value);
    } else {
      assert(size == sizeof(T));
      memcpy(&result, value, sizeof(T));
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const Field& field);
};

class IntField : public Field {
 public:
  IntField() : Field(FieldType::INT, sizeof(int)) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
  std::string to_string() const override { return "INT"; }
};

class CharField : public Field {
 public:
  CharField() : Field(FieldType::CHAR, sizeof(char)) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
  std::string to_string() const override { return "CHAR"; }
};

class DateField : public Field {
 public:
  DateField() : Field(FieldType::DATE, strlen("1999/99/99") * sizeof(char)) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
  std::string to_string() const override { return "DATE"; }
};

class Float32Field : public Field {
 public:
  Float32Field() : Field(FieldType::FLOAT32, sizeof(float)) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
  std::string to_string() const override { return "FLOAT32"; }
};

class Float64Field : public Field {
 public:
  Float64Field() : Field(FieldType::FLOAT64, sizeof(double)) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
  std::string to_string() const override { return "FLOAT64"; }
};

class BooleanField : public Field {
 public:
  BooleanField() : Field(FieldType::BOOLEAN, sizeof(bool)) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
  std::string to_string() const override { return "BOOLEAN"; }
};

class VarcharField : public Field {
 private:
  const std::size_t size;

 public:
  VarcharField(const std::size_t size)
      : Field(FieldType::VARCHAR, size * sizeof(char)), size(size) {}
  Byte* stringToBytes(const std::string& value) const override;
  std::string bytesToString(const Byte* value) const override;
  const std::size_t getSize() const { return size; }
  std::string to_string() const override {
    return "VARCHAR(" + std::to_string(size) + ")";
  }
};

class FieldCreator {
 public:
  static std::shared_ptr<Field> createField(const FieldType& type,
                                            const std::size_t size = 1);

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
                const std::size_t size = 1);
  void addField(const std::string& field_name, const std::string& type);
  std::vector<std::shared_ptr<Field>> getFields() const { return fields; };
  std::size_t size() const;

  class Iterator {
   private:
    typename std::vector<std::string>::const_iterator name_it;
    typename std::vector<std::shared_ptr<Field>>::const_iterator field_it;

   public:
    Iterator(
        typename std::vector<std::string>::const_iterator name_it,
        typename std::vector<std::shared_ptr<Field>>::const_iterator field_it)
        : name_it(name_it), field_it(field_it) {}

    std::pair<const std::string&, const std::shared_ptr<Field>&> operator*()
        const;

    Iterator& operator++();

    bool operator!=(const Iterator& other) const;
    bool operator==(const Iterator& other) const;
  };

  Iterator begin() const;
  Iterator end() const;

  std::string to_string() const;
  std::string to_table(const std::size_t min_name_length = 10,
                       const std::size_t min_type_length = 10) const;

  friend std::ostream& operator<<(std::ostream& os,
                                  const DataTypes& data_types);
};

#endif
