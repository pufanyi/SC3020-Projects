#ifndef RECORD_H
#define RECORD_H

#include "data_ptr.h"
#include "schema.h"

class Record : public DataPtr {
 private:
  std::shared_ptr<Schema> _schema;

 public:
  Record(const BlockPtr &block_ptr, off_t offset,
         std::shared_ptr<Schema> schema)
      : DataPtr(block_ptr, offset), _schema(schema) {}

  Record(const Record &record) = default;

  std::size_t size() const override;

  friend std::ostream &operator<<(std::ostream &os, const Record &record);

  const Byte *getData() const;
  std::string toString() const;
};

#endif  // RECORD_H
