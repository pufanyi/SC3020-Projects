#ifndef RECORD_H
#define RECORD_H

#include "data_ptr.h"
#include "schema.h"

class Record : public DataPtr {
 private:
  class RecordPtr {
   private:
    BlockPtr _block;
    std::size_t _pos;

   public:
    RecordPtr() : _block(), _pos(0) {}
    RecordPtr(const BlockPtr &block, std::size_t pos)
        : _block(block), _pos(pos) {}
    RecordPtr(const Byte *bytes, const std::shared_ptr<std::fstream> &file,
              const std::shared_ptr<BlockBuffer> &buffer,
              const std::shared_ptr<Schema> &schema) {
      load(bytes, file, buffer, schema);
    }

    const BlockPtr &block() const { return _block; }
    const std::size_t pos() const { return _pos; }

    void save(Byte *bytes) const;
    void load(const Byte *bytes, const std::shared_ptr<std::fstream> &file,
              const std::shared_ptr<BlockBuffer> &buffer,
              const std::shared_ptr<Schema> &schema);
    static std::size_t size() {
      return sizeof(BlockIndex) + sizeof(std::size_t);
    }

    bool is_nullptr() const;
  };

  std::shared_ptr<Schema> _schema;
  mutable std::shared_ptr<RecordPtr> _next;

 public:
  Record(const BlockPtr &block_ptr, off_t offset, std::shared_ptr<Schema> schema)
      : DataPtr(block_ptr, offset), _schema(schema) {}

  Record(const Record &record) = default;

  std::size_t record_header_size() const {
    return sizeof(BlockIndex) + RecordPtr::size();
  }

  std::size_t size() const override {
    return record_header_size() + data_size();
  }

  friend std::ostream &operator<<(std::ostream &os, const Record &record);

  std::shared_ptr<Record> next() const;
  void set_next(const std::shared_ptr<Record> &next);

  const Byte *getData() const { return getBytes() + RecordPtr::size(); }

  std::size_t data_size() const { return _schema->row_size(); }
};

#endif  // RECORD_H
