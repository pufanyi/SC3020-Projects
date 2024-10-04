#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <fstream>
#include <vector>

#include "block_ptr.h"
#include "buffer.h"
#include "fields.h"
#include "file_manager.h"
#include "record.h"

class DatabaseManager {
  std::shared_ptr<FileManager> file_manager;
  std::shared_ptr<Schema> schema;

 public:
  DatabaseManager(const std::string &file_name, bool create_new = false,
                  std::size_t max_blocks_cached = 100);
  std::vector<Record> load_from_txt(const std::string &file_name,
                                    const std::string &schema_name,
                                    const std::string &dtypes,
                                    const std::string &delimiter = ",");

  // Be cautions here, dtypes is different from load_from_txt
  // When using load_from_txt, you only need to pass in "INT, FLOAT32, ..."
  // The field name is read from the header of the txt file
  // When you load from a .db file, you need to pass in the field name
  // "field1 INT, field2 FLOAT32, ..."
  std::vector<Record> load_from_db(const std::string &schema_name,
                                   const std::string &dtypes,
                                   const size_t num_records);

  std::vector<Record> linear_scan(const std::string &field_name,
                                  const std::string &dtypes, const float low,
                                  const float high);
  std::shared_ptr<Schema> get_schema() const { return schema; }
  std::shared_ptr<FileManager> get_file_manager() const { return file_manager; }
};

#endif  // DATABASE_MANAGER_H
