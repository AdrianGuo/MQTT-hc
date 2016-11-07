/*
 * ZbHelper.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: taho
 */

#include <ZbDef/ZbHelper.hpp>
#include <string>

String
HexToString(
    const u8_p pbyHex,
    int_t iLength
){
  std::string ret;
  for (int i = 0; i < iLength; ++i)
  {
    static const char* HexConverter = "0123456789ABCDEF";
    ret += HexConverter[ ( pbyHex[i] & 0xF0 ) >> 4 ];
    ret += HexConverter[ ( pbyHex[i] & 0x0F ) >> 0 ];
  }
  return String(ret.c_str());
}

u16_t
LittleWord(
    u8_p* pbyBuffer
) {
    u16_t wRet;
    wRet = **pbyBuffer << 8;
    ++*pbyBuffer;
    wRet |= (**pbyBuffer);
    ++*pbyBuffer;
    return wRet;
}

u16_t
BigWord(
    u8_p* pbyBuffer
) {
    u16_t wRet;
    wRet = **pbyBuffer;
    ++*pbyBuffer;
    wRet |= (**pbyBuffer << 8);
    ++*pbyBuffer;
    return wRet;
}
