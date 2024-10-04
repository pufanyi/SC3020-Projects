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
  std::size_t _row_size;

 public:
  Schema() = delete;
  Schema(const std::string &name, const DataTypes &data_types)
      : schema_name(name),
        data_types(data_types),
        _row_size(data_types.size()) {}
  Schema(const std::string &name,
         const std::vector<std::pair<std::string, std::string>> &dtypes);
  Schema(const std::string &name, const std::vector<std::string> &dtypes);
  Schema(const std::string &name, const std::string &dtypes);

  const DataTypes &dtypes() const { return data_types; }
  const std::size_t row_size() const { return _row_size; }

  const BlockIndex &operator[](const std::string &field_name) const;
};

#endif  // SCHEMA_H
