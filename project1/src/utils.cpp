#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#elif __linux__ || __APPLE__
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace SystemUtils {
std::size_t __get_system_block_size() {
#ifdef _WIN32
  // Get the block size on Windows
  DWORD sectorsPerCluster, bytesPerSector;
  GetDiskFreeSpace(nullptr, &sectorsPerCluster, &bytesPerSector, nullptr,
                   nullptr);
  return (std::size_t)sectorsPerCluster * (std::size_t)bytesPerSector;
#elif __linux__ || __APPLE__
  // Get the block size of the filesystem on Linux
  struct stat fi;
  stat("/", &fi);
  return (std::size_t)fi.st_blksize;
#else
  std::cerr << "Error: Unsupported operating system when trying to get the "
               "block size."
            << std::endl;
  return 4096;
#endif
}
}  // namespace SystemUtils
