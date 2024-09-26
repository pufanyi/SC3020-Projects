#include <gtest/gtest.h>

#include "schema.h"

TEST(SchemaTest, CreateSchemaTest) {
  Schema schema = Schema(
      "test_schema",
      "int_field INT, float_field FLOAT32, date_field DATE, boolean_field "
      "BOOLEAN, char_field CHAR, varchar_field VARCHAR(10)");
  EXPECT_EQ(
      schema.dtypes().to_string(),
      "int_field INT,\nfloat_field FLOAT32,\ndate_field DATE,\nboolean_field "
      "BOOLEAN,\nchar_field CHAR,\nvarchar_field VARCHAR(10)");
  EXPECT_EQ(schema.row_size(), sizeof(int) + sizeof(float) + 10 * sizeof(char) +
                                   strlen("1999/99/99") * sizeof(char) +
                                   sizeof(bool) + sizeof(char));
}
