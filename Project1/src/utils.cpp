#include "utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>


int getSystemBlockSize()
{
    // Get the block size of the filesystem
    struct stat fi;
    stat("/", &fi);
    return fi.st_blksize;
}
