#include "database_manager.h"

DatabaseManager::DatabaseManager(const std::string &file_name, bool create_new,
                                 std::size_t max_blocks_cached)
    : file_manager(std::make_shared<FileManager>(file_name, create_new,
                                                 max_blocks_cached)) {}

const std::vector<Record> &DatabaseManager::load_from_txt(
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
  this->schema = schema_ptr;
  // Get basic info of the schema
  const DataTypes &data_types = schema.dtypes();
  std::vector<std::shared_ptr<Field>> fields = data_types.getFields();
  size_t row_size = schema.row_size();
  size_t num_records_per_block = BLOCK_SIZE / row_size;
  size_t record_num = 0;
  size_t record_num_curr = 0;
  BlockPtr block_ptr;
  // std::vector<Record> records;
  records = std::make_shared<std::vector<Record>>();

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
    records->push_back(record);
    record_num++;
    record_num_curr++;
  }
  this->schema = schema_ptr;
  return *this->records;
}

const std::vector<Record> &DatabaseManager::load_from_db(
    const std::string &schema_name, const std::string &dtypes,
    const size_t num_records) {
  std::vector<BlockPtr> block_ptrs = file_manager->getPtrs();
  Schema schema = Schema(schema_name, dtypes);
  std::shared_ptr<Schema> schema_ptr = std::make_shared<Schema>(schema);
  this->schema = schema_ptr;
  this->records = std::make_shared<std::vector<Record>>();
  // std::vector<Record> records;
  size_t row_size = schema.row_size();
  size_t num_records_per_block = BLOCK_SIZE / row_size;
  size_t record_num = 0;
  for (int i = 0; i < block_ptrs.size(); i++) {
    BlockPtr block_ptr = block_ptrs[i];
    int begin = 0;
    for (int j = 0; j < num_records_per_block; j++) {
      if (record_num >= num_records) {
        break;
      }
      Record record = Record(block_ptr, begin, schema_ptr);
      records->push_back(record);
      begin += row_size;
      record_num++;
    }
  }
  return *this->records;
}

std::vector<Record> DatabaseManager::linear_scan(const std::string &field_name,
                                                 const std::string &dtypes,
                                                 const float low,
                                                 const float high) {
  std::vector<Record> records;
  std::shared_ptr<Schema> schema =
      std::make_shared<Schema>(Schema("schema", dtypes));
  std::vector<std::string> field_names = schema->dtypes().getFieldNames();
  std::vector<std::shared_ptr<Field>> fields = schema->dtypes().getFields();
  size_t row_size = schema->row_size();
  size_t num_records_per_block = BLOCK_SIZE / row_size;
  size_t record_num = 0;
  for (int i = 0; i < file_manager->num_blocks(); i++) {
    BlockPtr block_ptr = file_manager->getPtr(i * BLOCK_SIZE);
    for (int j = 0; j < num_records_per_block; j++) {
      Record record = Record(block_ptr, j * row_size, schema);
      const Byte *bytes = record.getData();
      int begin = 0;
      int field_index = 0;
      for (auto &field : fields) {
        if (field_names[field_index] == field_name) {
          Byte *single_field = new Byte[field->getSize()];
          std::copy(bytes + begin, bytes + begin + field->getSize(),
                    single_field);
          std::string value = field->bytesToString(single_field);
          float float_value = std::stof(value);
          if (float_value >= low && float_value <= high) {
            records.push_back(record);
          }
        }
        begin += field->getSize();
        field_index++;
      }
      record_num++;
    }
  }
  return records;
}

std::shared_ptr<BPlusTree> DatabaseManager::build_index(
    const std::string &file_name, const std::string &index_name,
    const IndexType index_type) {
  if (this->records == nullptr) {
    throw std::runtime_error("No records loaded");
  }
  std::cerr << "Building index" << std::endl;
  std::shared_ptr<BPlusTree> b_plus_tree = std::make_shared<BPlusTree>(
      index_type, index_name, file_name, schema, *records);
  return b_plus_tree;
}

std::shared_ptr<BPlusTree> DatabaseManager::load_index(
    const std::string &file_name, const std::string &index_name,
    const IndexType index_type) {
  std::shared_ptr<BPlusTree> b_plus_tree = std::make_shared<BPlusTree>(
      true, index_type, index_name, file_name, schema);
  return b_plus_tree;
}
