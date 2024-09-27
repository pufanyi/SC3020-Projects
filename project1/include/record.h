#ifndef RECORD_H
#define RECORD_H

#include "data_ptr.h"
#include "schema.h"

class Record : public DataPtr {
 private:
  std::shared_ptr<Schema> _schema;

 public:
  Record(BlockPtr &block_ptr, off_t offset, std::shared_ptr<Schema> schema)
      : DataPtr(block_ptr, offset), _schema(schema) {}

  Record(const Record &record) = default;

  std::size_t size() override { return _schema->row_size(); }

  friend std::ostream &operator<<(std::ostream &os, const Record &record);
};

#endif  // RECORD_H
