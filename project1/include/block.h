#ifndef BLOCK_H
#define BLOCK_H

#include <cstring>
#include <fstream>
#include <memory>

#include "utils.h"

class BlockData {
 private:
  Byte *data;
  std::streamoff _offset;
  std::shared_ptr<std::fstream> _file;

 public:
  BlockData(const std::streamoff offset = 0,
            std::shared_ptr<std::fstream> file = nullptr);
  BlockData(const Byte *data, std::streamoff offset = 0,
            std::shared_ptr<std::fstream> file = nullptr);
  BlockData(const BlockData &block_data) =
      delete;  // Copy constructor will cause a lot of problems, so we delete it

  virtual ~BlockData();

  const Byte &operator[](std::size_t index) const;
  Byte &operator[](std::size_t index);

  void save() const;

  const Byte *getData() const;

  friend class BlockPtr;
  friend class BlockBuffer;
};

#endif  // BLOCK_H
