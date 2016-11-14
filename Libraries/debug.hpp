#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typedefs.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define DEBUG1(x)               std::cout << "DEBUG1: " << std::setw(15) << __FILENAME__ \
                                          << ":" << std::setw(4) << __LINE__ \
                                          << ": " << x << std::endl;

#define DEBUG2(x, args ...)     printf("DEBUG2: %15s:%4d: " x "\n", \
                                        __FILENAME__, __LINE__, ##args)
#ifdef MT7688
namespace std {
    std::string to_string(int i);
    int stoi(std::string string);
}
#endif
#endif /* !DEBUG_HPP_ */
