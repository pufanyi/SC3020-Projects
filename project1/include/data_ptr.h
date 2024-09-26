#ifndef DATA_PTR_H
#define DATA_PTR_H

#include "block.h"
#include "fields.h"

class DataPtr {
 private:
  std::shared_ptr<Field> _field;
  std::shared_ptr<BlockData> _block_data;
  std::size_t _offset;

 public:
};

#endif  // DATA_PTR_H