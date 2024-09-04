#ifndef FIELD_H
#define FIELD_H

#include <string>

#include "utils.h"

enum class FieldType {
  INT,
  CHAR,
  // VARCHAR, # Maybe include this later, for simplicity, I will not include
  // this for now
  DATE,
  FLOAT,
  BOOLEAN,
};

class Field {
 protected:
  FieldType type;
  int size;

 public:
  Field() = default;
  Field(FieldType type, int& size);
  ~Field() = default;
  virtual Byte* stringToBytes(const std::string& value) = 0;
  virtual std::string bytesToString(const Byte* value) = 0;
};

class IntField : public Field {
 public:
  IntField();
  Byte* stringToBytes(const std::string& value) override;
  std::string bytesToString(const Byte* value) override;
};

class CharField : public Field {
 public:
  CharField();
  Byte* stringToBytes(const std::string& value) override;
  std::string bytesToString(const Byte* value) override;
};

class DateField : public Field {
 public:
  DateField();
  Byte* stringToBytes(const std::string& value) override;
  std::string bytesToString(const Byte* value) override;
};

class FloatField : public Field {
 public:
  FloatField();
  Byte* stringToBytes(const std::string& value) override;
  std::string bytesToString(const Byte* value) override;
};

class BooleanField : public Field {
 public:
  BooleanField();
  Byte* stringToBytes(const std::string& value) override;
  std::string bytesToString(const Byte* value) override;
};

class FieldCreator {
 public:
  static Field* createField(FieldType& type);
};

#endif
