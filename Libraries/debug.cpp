/*
 * debug.cpp
 *
 *  Created on: Nov 14, 2016
 *      Author: taho
 */
#include <string>
#include <sstream>
#include <cstdlib>

#ifdef MT7688
namespace std {
    std::string to_string(int i) {
        std::stringstream ss;
        ss << i;
        return ss.str();
    }

    int stoi(std::string string) {
        return atoi(string.c_str());
    }
}
#endif



