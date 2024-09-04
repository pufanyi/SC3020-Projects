#include <gtest/gtest.h>

#include "fields.h"

TEST(FieldTest, IntFieldTest) {
  FieldType type = FieldType::INT;
  Field *field = FieldCreator::createField(type);
  std::string value = "1234";
  Byte *bytes = field->stringToBytes(value);
  std::string result = field->bytesToString(bytes);
  EXPECT_EQ(value, result);

  int val = 1234;
  bytes = field->valueToBytes(val);
  field->bytesToValue(bytes, val);
  EXPECT_EQ(val, 1234);
}

TEST(FieldTest, FloatFieldTest) {
  FieldType type = FieldType::FLOAT;
  Field *field = FieldCreator::createField(type);
  std::string value = "1.430000";
  Byte *bytes = field->stringToBytes(value);
  std::string result = field->bytesToString(bytes);
  EXPECT_EQ(value, result);
}

TEST(FieldTest, DateFieldTest) {
  FieldType type = FieldType::DATE;
  Field *field = FieldCreator::createField(type);
  std::string value = "22/12/2022";
  Byte *bytes = field->stringToBytes(value);
  std::string result = field->bytesToString(bytes);
  EXPECT_EQ(value, result);
}

TEST(FieldTest, BooleanFieldTest) {
  FieldType type = FieldType::BOOLEAN;
  Field *field = FieldCreator::createField(type);
  std::string value = "1";
  Byte *bytes = field->stringToBytes(value);
  std::string result = field->bytesToString(bytes);
  EXPECT_EQ(value, result);
}

TEST(FieldTest, CharFieldTest) {
  FieldType type = FieldType::CHAR;
  Field *field = FieldCreator::createField(type);
  std::string value = "a";
  Byte *bytes = field->stringToBytes(value);
  std::string result = field->bytesToString(bytes);
  EXPECT_EQ(value, result);
}
