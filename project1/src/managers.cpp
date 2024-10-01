#include "managers.h"

#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <sstream>

#include "record.h"
#include "utils.h"

FileManager::FileManager(const std::string &file_name, bool create_new,
                         std::size_t max_blocks_cached)
    : _file_name(file_name),
      buffer(std::make_shared<BlockBuffer>(max_blocks_cached)) {
  std::ios_base::openmode mode =
      std::ios::in | std::ios::out | std::ios::binary;
  if (create_new) {
    mode |= std::ios::trunc;
  }

  file = std::make_shared<std::fstream>(file_name, mode);
  if (!file->is_open()) {
    throw std::runtime_error("Error opening file");
  }

  file->seekg(0, std::ios::end);
  std::streampos fileSize = file->tellg();
  if (fileSize == -1) {
    throw std::runtime_error("Error seeking file");
  }

  if (create_new) {
    _num_blocks = 0;
  } else {
    if (fileSize % BLOCK_SIZE != 0) {
      std::stringstream ss;
      ss << "File size is not a multiple of block size, file size: " << fileSize
         << ", block size: " << BLOCK_SIZE;
      throw std::runtime_error(ss.str());
    }
    _num_blocks = fileSize / BLOCK_SIZE;
  }
}

BlockPtr FileManager::newPtr() {
  file->seekp(0, std::ios::end);
  std::streampos fileEnd = file->tellp();
  if (fileEnd == -1) {
    throw std::runtime_error("Error seeking to end of file");
  }

  Byte newBlock[BLOCK_SIZE];

  file->write(newBlock, BLOCK_SIZE);
  if (file->fail()) {
    throw std::runtime_error("Error writing new block to file");
  }

  file->flush();
  if (file->fail()) {
    throw std::runtime_error("Error flushing file");
  }

  this->_num_blocks++;

  return BlockPtr(file, fileEnd, buffer);
}

std::vector<BlockPtr> FileManager::getPtrs() const {
  std::vector<BlockPtr> ptrs;
  for (std::size_t i = 0; i < _num_blocks; i++) {
    ptrs.push_back(BlockPtr(file, i * BLOCK_SIZE, buffer));
  }
  return ptrs;
}

BlockPtr FileManager::getPtr(const BlockIndex &offset) {
  return BlockPtr(file, offset, buffer);
}
BlockPtr FileManager::getPtr(const Byte *bytes) {
  return BlockPtr(file, bytes, buffer);
}

DatabaseManager::DatabaseManager(const std::string &file_name, bool create_new,
                                 std::size_t max_blocks_cached)
    : file_manager(std::make_shared<FileManager>(file_name, create_new,
                                                 max_blocks_cached)) {}

std::vector<Record> DatabaseManager::load_from_txt(
    const std::string &file_name, const std::string &schema_name,
    const std::string &dtypes, const std::string &delimiter) {
  // Assume that the user pass in a string of dtypes
  // separated by commas
  std::vector<std::string> dtypes_vector;
  std::istringstream iss(dtypes);
  std::string dtype;
  while (std::getline(iss, dtype, ',')) {
    dtypes_vector.push_back(dtype);
  }

  // Create a stream to read the txt file
  std::ifstream file(file_name);
  std::string str;
  std::string file_contents;

  // Read the header of the file
  std::getline(file, str);
  str = trim(str);
  std::string header = "";
  std::vector<std::string> header_chunk = split(str, delimiter[0]);
  // Create a schema from the header_chunk
  // Combine the field name and dtype
  for (int i = 0; i < header_chunk.size(); i++) {
    header += header_chunk[i] + " " + dtypes_vector[i] + ",";
  }
  Schema schema = Schema(schema_name, header);
  std::shared_ptr<Schema> schema_ptr = std::make_shared<Schema>(schema);
  // Get basic info of the schema
  DataTypes data_types = schema.dtypes();
  std::vector<std::shared_ptr<Field>> fields = data_types.getFields();
  size_t row_size = schema.row_size();
  size_t num_records_per_block = BLOCK_SIZE / row_size;
  size_t record_num = 0;
  size_t record_num_curr = 0;
  BlockPtr block_ptr;
  std::vector<Record> records;

  while (std::getline(file, str)) {
    size_t num_block = file_manager->num_blocks();
    size_t capacities = num_block * num_records_per_block;
    if (record_num >= capacities) {
      block_ptr = file_manager->newPtr();
      record_num_curr = 0;
    }

    str = trim(str);
    std::vector<std::string> data_chunk = split(str, delimiter[0]);
    Record record = Record(block_ptr, record_num_curr * row_size, schema_ptr);
    // Create a record from the data_chunk
    int begin = 0;
    for (int i = 0; i < data_chunk.size(); i++) {
      // A hack here, some value is empty,
      // fill with 999 because the fields are all int or float
      if (data_chunk[i].size() == 0) {
        data_chunk[i] = "999";
      }
      Byte *bytes = fields[i]->stringToBytes(data_chunk[i]);
      record.store(bytes, begin, begin + fields[i]->getSize());
      begin += fields[i]->getSize();
    }
    records.push_back(record);
    record_num++;
    record_num_curr++;
  }
  this->schema = schema_ptr;
  return records;
}

std::vector<Record> DatabaseManager::load_from_db(
    const std::string &file_name) {
  std::vector<Record> records;
  return records;
}
