#include "schema.h"

#include <sstream>

Schema::Schema(const std::string &name,
               const std::vector<std::pair<std::string, std::string>> &dtypes)
    : schema_name(name), _length(0) {
  for (const auto &[field_name, type] : dtypes) {
    data_types.addField(field_name, type);
  }
  _row_size = data_types.size();
}

Schema::Schema(const std::string &name, const std::vector<std::string> &dtypes)
    : schema_name(name), _length(0) {
  for (const auto &s : dtypes) {
    std::istringstream iss(s);
    std::string field_name;
    std::string type;
    iss >> field_name >> type;
    data_types.addField(field_name, type);
  }
  _row_size = data_types.size();
}

Schema::Schema(const std::string &name, const std::string &dtypes)
    : schema_name(name), _length(0) {
  std::istringstream iss(dtypes);
  std::string field_data;
  while (std::getline(iss, field_data, ',')) {
    std::istringstream field_stream(field_data);
    std::string field_name, type;
    field_stream >> field_name >> type;
    data_types.addField(field_name, type);
  }
  _row_size = data_types.size();
}
