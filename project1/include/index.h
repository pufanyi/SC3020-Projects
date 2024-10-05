#ifndef INDEX_H
#define INDEX_H

#include <cstring>
#include <memory>

#include "utils.h"

enum class IndexType { INT, STRING, FLOAT32, FLOAT64 };

class Index {
 public:
  Index() = default;
  virtual ~Index() = default;

  virtual bool smallerThan(const Index& index) const = 0;

  bool operator<(const Index& index) const { return this->smallerThan(index); }
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

class IntIndex : public Index {
 private:
  int _value;

 public:
  IntIndex() : _value(0) {}
  IntIndex(int value) : _value(value) {}
  ~IntIndex() = default;

  bool smallerThan(const Index& index) const override {
    return _value < dynamic_cast<const IntIndex&>(index)._value;
  }

  std::size_t size() const override { return sizeof(int); }
  void save(Byte* bytes) const override { memcpy(bytes, &_value, sizeof(int)); }
  void load(const Byte* bytes) override { memcpy(&_value, bytes, sizeof(int)); }
};

class StringIndex : public Index {
 private:
  std::string _value;

 public:
  StringIndex() : _value("") {}
  StringIndex(const std::string& value) : _value(value) {}
  ~StringIndex() = default;

  bool smallerThan(const Index& index) const override {
    return _value < dynamic_cast<const StringIndex&>(index)._value;
  }

  std::size_t size() const override { return _value.size(); }
  void save(Byte* bytes) const override {
    memcpy(bytes, _value.c_str(), _value.size());
  }
  void load(const Byte* bytes) override {
    _value = std::string(reinterpret_cast<const char*>(bytes));
  }
};

class Float32Index : public Index {
 private:
  float _value;

 public:
  Float32Index() : _value(0) {}
  Float32Index(float value) : _value(value) {}
  ~Float32Index() = default;

  bool smallerThan(const Index& index) const override {
    return _value < dynamic_cast<const Float32Index&>(index)._value;
  }

  std::size_t size() const override { return sizeof(float); }
  void save(Byte* bytes) const override {
    memcpy(bytes, &_value, sizeof(float));
  }
  void load(const Byte* bytes) override {
    memcpy(&_value, bytes, sizeof(float));
  }
};

class Float64Index : public Index {
 private:
  double _value;

 public:
  Float64Index() : _value(0) {}
  Float64Index(double value) : _value(value) {}
  ~Float64Index() = default;

  bool smallerThan(const Index& index) const override {
    return _value < dynamic_cast<const Float64Index&>(index)._value;
  }

  std::size_t size() const override { return sizeof(double); }
  void save(Byte* bytes) const override {
    memcpy(bytes, &_value, sizeof(double));
  }
  void load(const Byte* bytes) override {
    memcpy(&_value, bytes, sizeof(double));
  }
};

std::shared_ptr<Index> createIndex(const IndexType& type);

std::size_t getIndexSize(const IndexType& type);
#endif  // INDEX_H
