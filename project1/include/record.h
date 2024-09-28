#ifndef RECORD_H
#define RECORD_H

#include "data_ptr.h"
#include "schema.h"

class RecordPtr {
 private:
  BlockPtr _block;
  std::size_t _pos;

 public:
  RecordPtr(const BlockPtr &block, std::size_t pos)
      : _block(block), _pos(pos) {}
  RecordPtr(const Byte *bytes, const std::shared_ptr<std::fstream> &file,
            const std::shared_ptr<BlockBuffer> &buffer,
            const std::shared_ptr<Schema> &schema) {
    load(bytes, file, buffer, schema);
  }

  const BlockPtr &block() const { return _block; }
  std::size_t pos() const { return _pos; }
  void save(Byte *bytes) const;
  void load(const Byte *bytes, const std::shared_ptr<std::fstream> &file,
            const std::shared_ptr<BlockBuffer> &buffer,
            const std::shared_ptr<Schema> &schema);
};

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
