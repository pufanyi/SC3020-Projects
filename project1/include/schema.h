#ifndef SCHEMA_H
#define SCHEMA_H

#include <memory>
#include <utility>
#include <vector>

#include "fields.h"

class Schema {
 private:
  DataTypes data_types;

 public:
  Schema() = delete;
  Schema(const DataTypes &data_types) : data_types(data_types) {}
  Schema(const std::vector<std::pair<std::string, std::string>> &dtypes);
  Schema(const std::vector<std::string> &dtypes);
  Schema(const std::string &dtypes);
};

#endif  // SCHEMA_H
