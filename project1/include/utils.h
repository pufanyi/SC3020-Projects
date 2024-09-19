#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <cstdio>
#include <vector>

namespace SystemUtils {
std::size_t __get_system_block_size();
}

const std::size_t BLOCK_SIZE = SystemUtils::__get_system_block_size();

using Byte = char;

constexpr std::size_t MAX_FILE_NAME_SIZE = 255;
constexpr std::size_t BLOCK_FILE_NAME_SIZE = MAX_FILE_NAME_SIZE + 1;

#endif  // UTILS_H
