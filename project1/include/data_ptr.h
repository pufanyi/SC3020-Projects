#ifndef DATA_PTR_H
#define DATA_PTR_H

#include <fstream>

#include "block.h"
#include "utils.h"

class DataPtr {
 private:
  std::fstream* file;
  std::streamoff offset;

 public:
  DataPtr(std::fstream* file, const std::streamoff& offset);
  DataPtr(const Byte* bytes);

  BlockData load() const;
  void store(const BlockData& block) const;
  Byte* getBytes() const;
  static DataPtr* fromBytes(const Byte* bytes);
  static std::size_t size();
};

#endif  // DATA_PTR_H
