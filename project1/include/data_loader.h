#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <iostream>

class DataLoader {
  std::string file_name;

public:
  DataLoader(const std::string &file_name) : file_name(file_name) {}
};

#endif
