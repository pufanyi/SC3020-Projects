#include "index.h"

std::shared_ptr<Index> createIndex(const IndexType& type) {
  switch (type) {
    case IndexType::INT:
      return std::make_shared<IntIndex>();
    case IndexType::STRING:
      return std::make_shared<StringIndex>();
    case IndexType::FLOAT32:
      return std::make_shared<Float32Index>();
    case IndexType::FLOAT64:
      return std::make_shared<Float64Index>();
    default:
      throw std::runtime_error("Invalid index type");
  }
}

std::size_t getIndexSize(const IndexType& type) {
  return createIndex(type)->size();
}
