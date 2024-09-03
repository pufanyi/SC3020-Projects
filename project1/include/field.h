#ifndef FIELD_H
#define FIELD_H

#include <string>

class Field {
 public:
  virtual ~Field() = default;
  virtual void parse(const std::string &field) = 0;
}

#endif
