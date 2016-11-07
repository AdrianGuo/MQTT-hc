#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typedefs.h>

#define DEBUG1(x)               std::cout << "DEBUG1: " << std::setw(40) << __FILE__ \
                                          << ":" << std::setw(4) << __LINE__ \
                                          << ":" << std::setw(25) << __FUNCTION__ \
                                          << ":" << std::setw(12) << (uint_t) pthread_self() \
                                          << ":" << x << std::endl;

#define DEBUG2(x, args ...)     printf("DEBUG2: %40s:%4d:%25s:%12u:" x "\n", \
                                        __FILE__, __LINE__, __FUNCTION__, (uint_t) pthread_self(), ##args)

#endif /* !DEBUG_HPP_ */
