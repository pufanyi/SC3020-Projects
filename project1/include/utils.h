#ifndef UTILS_H
#define UTILS_H

#include <cstdio>

std::size_t getSystemBlockSize();

const std::size_t BLOCK_SIZE = getSystemBlockSize();

#endif  // UTILS_H
