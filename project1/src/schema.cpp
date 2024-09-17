#include "schema.h"

#include <sstream>

Schema::Schema(const std::vector<std::pair<std::string, std::string>> &dtypes) {
  for (const auto &[field_name, type] : dtypes) {
    data_types.addField(field_name, type);
  }
}

Schema::Schema(const std::vector<std::string> &dtypes) {
  for (const auto &s : dtypes) {
    std::istringstream iss(s);
    std::string field_name;
    std::string type;
    iss >> field_name >> type;
    data_types.addField(field_name, type);
  }
}

Schema::Schema(const std::string &dtypes) {
  std::istringstream iss(dtypes);
  std::string field_data;
  while (std::getline(iss, field_data, ',')) {
    std::istringstream field_stream(field_data);
    std::string field_name, type;
    field_stream >> field_name >> type;
    data_types.addField(field_name, type);
  }
}
