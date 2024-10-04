#ifndef INDEX_H
#define INDEX_H

#include "utils.h"

class Index {
 public:
  Index() = default;
  virtual ~Index() = default;
  virtual bool operator<(const Index& index) const = 0;

  bool operator>(const Index& index) const { return index < *this; }

  bool operator<=(const Index& index) const { return !(*this > index); }

  bool operator>=(const Index& index) const { return !(*this < index); }

  bool operator==(const Index& index) const {
    return !(*this < index) && !(index < *this);
  }

  virtual std::size_t size() const = 0;
  virtual void save(Byte* bytes) const = 0;
  virtual void load(const Byte* bytes) = 0;
};

#endif  // INDEX_H
