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

#endif  // UTILS_H
