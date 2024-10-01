#ifndef MANAGERS_H
#define MANAGERS_H

#include <fstream>
#include <vector>

#include "block_ptr.h"
#include "buffer.h"
#include "fields.h"
#include "record.h"

class FileManager {
  std::shared_ptr<std::fstream> file;
  const std::string _file_name;
  std::size_t _num_blocks;
  std::shared_ptr<BlockBuffer> buffer;

 public:
  FileManager(const std::string &file_name, bool create_new = false,
              std::size_t max_blocks_cached = 100);

  BlockPtr newPtr();
  BlockPtr getPtr(const BlockIndex &offset);
  BlockPtr getPtr(const Byte *bytes);

  const std::string &file_name() const { return _file_name; }
  std::size_t num_blocks() const { return _num_blocks; }

  std::vector<BlockPtr> getPtrs() const;
};

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

  std::vector<Record> load_from_db(const std::string &file_name);
  std::shared_ptr<Schema> get_schema() const { return schema; }
  std::shared_ptr<FileManager> get_file_manager() const { return file_manager; }
};

#endif  // MANAGERS_H
