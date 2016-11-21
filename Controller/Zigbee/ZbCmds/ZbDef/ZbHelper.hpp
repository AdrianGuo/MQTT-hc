/*
 * ZbHelper.hpp
 *
 *  Created on: Nov 2, 2016
 *      Author: taho
 */

#ifndef ZBHELPER_HPP_
#define ZBHELPER_HPP_

#include <typedefs.h>
#include <String.hpp>

String HexToString(const u8_p pbyHex, int_t iLength);
u16_t LittleWord(u8_p* pbyBuffer, bool inc = true);
u16_t BigWord(u8_p* pbyBuffer, bool inc = true);

#endif /* ZBHELPER_HPP_ */
