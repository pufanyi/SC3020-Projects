#include "data_ptr.h"

void DataPtr::store(const std::string& value) {
Byte* bytes = _field->stringToBytes(value);
memcpy(&(*_block_data)[_offset], bytes, _field->getSize());
}

void DataPtr::store(const Byte* bytes) {
    memcpy(&(*_block_data)[_offset], bytes, _field->getSize());
}


Byte *DataPtr::getBytes() const {
    return &(*_block_data)[_offset];
}

std::string DataPtr::load_str() {
    return _field->bytesToString(&(*_block_data)[_offset]);
}
