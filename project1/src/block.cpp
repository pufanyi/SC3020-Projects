#include "block.h"

#include <iostream>
#include <stdexcept>

BlockData::BlockData(const std::streamoff offset,
                     std::shared_ptr<std::fstream> file)
    : data(new Byte[BLOCK_SIZE]), _offset(offset), _file(file) {
  memset(data, 0, BLOCK_SIZE);
}

BlockData::BlockData(const Byte *data, std::streamoff offset,
                     std::shared_ptr<std::fstream> file)
    : data(new Byte[BLOCK_SIZE]), _offset(offset), _file(file) {
  memset(this->data, 0, BLOCK_SIZE);
  if (data != nullptr) {
    memcpy(this->data, data, BLOCK_SIZE * sizeof(Byte));
  }
}

const Byte *BlockData::getData() const { return data; }

const Byte &BlockData::operator[](std::size_t index) const {
  if (index >= BLOCK_SIZE) {
    throw std::out_of_range("Index out of range");
  }
  return data[index];
}

Byte &BlockData::operator[](std::size_t index) {
  if (index >= BLOCK_SIZE) {
    throw std::out_of_range("Index out of range");
  }
  return data[index];
}

void BlockData::save() const {
  if (_file == nullptr) {
    throw std::runtime_error("File not set");
  }
  if (!_file->is_open()) {
    throw std::runtime_error("File is not open");
  }
  _file->seekp(_offset, std::ios::beg);
  if (_file->fail()) {
    std::cerr << "Error seeking to block at offset " << _offset << std::endl;
    std::cerr << "File state: " << _file->rdstate() << std::endl;
    if (_file->eof()) std::cerr << "End of file reached" << std::endl;
    if (_file->bad())
      std::cerr << "Bad bit set (possible I/O error)" << std::endl;
    throw std::runtime_error("Error seeking to block");
  }
  _file->write(data, BLOCK_SIZE);
  if (_file->fail()) {
    throw std::runtime_error("Error writing block");
  }
  _file->flush();
}

BlockData::~BlockData() {
  if (_file != nullptr) {
    save();
  }
  delete[] data;
}
