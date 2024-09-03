#ifndef UTILS_H
#define UTILS_H

#include <cstdio>

namespace SystemUtils {
std::size_t __get_system_block_size();
}

const std::size_t BLOCK_SIZE = SystemUtils::__get_system_block_size();

#endif  // UTILS_H
