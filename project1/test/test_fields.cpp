#include <gtest/gtest.h>

#include "fields.h"

TEST(FieldTest, IntFieldTest) {
  FieldType type = FieldType::INT;
  auto field = FieldCreator::createField(type);
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
  auto field = FieldCreator::createField(type);
  std::string value = "1.430000";
  Byte *bytes = field->stringToBytes(value);
  std::string result = field->bytesToString(bytes);
  EXPECT_EQ(value, result);
}

TEST(FieldTest, DateFieldTest) {
  FieldType type = FieldType::DATE;
  auto field = FieldCreator::createField(type);
  std::string value = "22/12/2022";
  Byte *bytes = field->stringToBytes(value);
  std::string result = field->bytesToString(bytes);
  EXPECT_EQ(value, result);
}

TEST(FieldTest, BooleanFieldTest) {
  FieldType type = FieldType::BOOLEAN;
  auto field = FieldCreator::createField(type);
  std::string value = "1";
  Byte *bytes = field->stringToBytes(value);
  std::string result = field->bytesToString(bytes);
  EXPECT_EQ(value, result);
}

TEST(FieldTest, CharFieldTest) {
  FieldType type = FieldType::CHAR;
  auto field = FieldCreator::createField(type);
  std::string value = "a";
  Byte *bytes = field->stringToBytes(value);
  std::string result = field->bytesToString(bytes);
  EXPECT_EQ(value, result);
}

TEST(FieldTest, VarcharFieldTest) {
  FieldType type = FieldType::VARCHAR;
  auto field = FieldCreator::createField(type, 10);
  std::string value = "abcdefgh";
  Byte *bytes = field->stringToBytes(value);
  std::string result = field->bytesToString(bytes);
  EXPECT_EQ(value, result);
}

TEST(FieldTest, VarcharInvalidFieldSizeTest) {
  FieldType type = FieldType::VARCHAR;
  auto field = FieldCreator::createField(type, 10);
  std::string value = "abcdefghijk";
  EXPECT_THROW(field->stringToBytes(value), std::invalid_argument);
}

TEST(FieldTest, DataTypesAppendTest) {
  DataTypes data_types;
  data_types.addField("int_field", FieldType::INT);
  data_types.addField("float_field", FieldType::FLOAT);
  data_types.addField("date_field", FieldType::DATE);
  data_types.addField("boolean_field", FieldType::BOOLEAN);
  data_types.addField("char_field", FieldType::CHAR);
  data_types.addField("varchar_field", FieldType::VARCHAR, 10);
  EXPECT_EQ(
      data_types.to_string(),
      "int_field INT,\nfloat_field FLOAT,\ndate_field DATE,\nboolean_field "
      "BOOLEAN,\nchar_field CHAR,\nvarchar_field VARCHAR(10)");

  const std::string expected_table =
      "+---------------+-------------+\n| Field Name    | Field Type  "
      "|\n+---------------+-------------+\n| int_field     | INT         |\n| "
      "float_field   | FLOAT       |\n| date_field    | DATE        |\n| "
      "boolean_field | BOOLEAN     |\n| char_field    | CHAR        |\n| "
      "varchar_field | VARCHAR(10) |\n+---------------+-------------+\n";
  EXPECT_EQ(data_types.to_table(), expected_table);

  std::ostringstream os;
  os << data_types;
  std::string data_types_table = os.str();
  EXPECT_EQ(data_types_table, expected_table);
}

TEST(FieldTest, DataTypesAppendWithStringTest) {
  DataTypes data_types;
  data_types.addField("int_field", "INT");
  data_types.addField("float_field", "FLOAT");
  data_types.addField("date_field", "Date");
  data_types.addField("boolean_field", "boolean");
  data_types.addField("char_field", "CHAR");
  data_types.addField("varchar_field", "VARCHAR(10)");
  EXPECT_EQ(
      data_types.to_string(),
      "int_field INT,\nfloat_field FLOAT,\ndate_field DATE,\nboolean_field "
      "BOOLEAN,\nchar_field CHAR,\nvarchar_field VARCHAR(10)");

  const std::string expected_table =
      "+---------------+-------------+\n| Field Name    | Field Type  "
      "|\n+---------------+-------------+\n| int_field     | INT         |\n| "
      "float_field   | FLOAT       |\n| date_field    | DATE        |\n| "
      "boolean_field | BOOLEAN     |\n| char_field    | CHAR        |\n| "
      "varchar_field | VARCHAR(10) |\n+---------------+-------------+\n";
  EXPECT_EQ(data_types.to_table(), expected_table);

  std::ostringstream os;
  os << data_types;
  std::string data_types_table = os.str();
  EXPECT_EQ(data_types_table, expected_table);
}
