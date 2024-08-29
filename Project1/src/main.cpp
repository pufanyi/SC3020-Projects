#include <iostream>
#include "utils.h"
using namespace std;

int main()
{
    std::cout << "Hello World!" << std::endl;
    std::cout << "System block size: " << getSystemBlockSize() << std::endl;
    return 0;
}
