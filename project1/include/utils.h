#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <vector>
#define GTEST_COUT std::cerr << "[          ] [ INFO ]"

namespace SystemUtils {
std::size_t __get_system_block_size();
}
extern size_t IO_TIMES;
extern size_t LOAD_PTR_TIMES;

std::vector<std::string> split(const std::string &s, char delimiter);
std::string trim(std::string line);

const std::size_t BLOCK_SIZE = SystemUtils::__get_system_block_size();

using Byte = char;
using BlockIndex = std::size_t;

constexpr std::size_t MAX_FILE_NAME_SIZE = 255;
constexpr std::size_t BLOCK_FILE_NAME_SIZE = MAX_FILE_NAME_SIZE + 1;

#define DEBUG 1

constexpr std::size_t BUFFER_SIZE = 500;

#endif  // UTILS_H
