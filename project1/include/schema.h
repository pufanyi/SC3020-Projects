#ifndef SCHEMA_H
#define SCHEMA_H

#include <memory>
#include <utility>
#include <vector>

#include "fields.h"

class Schema {
 private:
  const std::string schema_name;
  DataTypes data_types;
  size_t _size;

 public:
  Schema() = delete;
  Schema(const std::string &name, const DataTypes &data_types)
      : schema_name(name), data_types(data_types), _size(data_types.size()) {}
  Schema(const std::string &name,
         const std::vector<std::pair<std::string, std::string>> &dtypes);
  Schema(const std::string &name, const std::vector<std::string> &dtypes);
  Schema(const std::string &name, const std::string &dtypes);

  const DataTypes &dtypes() const { return data_types; }
  const size_t size() const { return _size; }
};

#endif  // SCHEMA_H
