/*******************************************************************************
*
* Copyright (c) 2016
* Lumi, JSC.
* All Rights Reserved
*
* FileName: HelperHc.hpp
*
* Author: TrungTQ
*
* Last Changed By:  TrungTQ (trungkstn@gmail.com)
* Revision:         1.0
* Last Changed:     Date: 2016-08-05 10:15:00 (Fri, 04 Aug 2016)
*
******************************************************************************/
#ifndef HELPER_HC_HPP_
#define HELPER_HC_HPP_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include <algorithm>
#include <unistd.h>
#include <string.h>

#include "Typedefs.h"
#include "LogPlus.hpp"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/**
 * @func   ptr
 * @brief  turn reference into pointer
 * @param  None
 * @retval None
 */
template<typename T>
inline T* ptr(T & obj) {
    return &obj;
}

/**
 * @func   ptr
 * @brief  obj is already pointer, return it
 * @param  None
 * @retval None
 */
template<typename T>
inline T* ptr(T * obj) {
    return obj;
}

/**
 * @func   GetHex
 * @brief  Convert Data to Hex format String
 * @param  None
 * @retval None
 */
template<typename T>
inline String
GetHex(
    T data,
    size_t hex_len = sizeof(T) << 1
) {
    static const_char_p digits = "0123456789ABCDEF";
    String rc(hex_len, '0');
    for (size_t i = 0, j = (hex_len - 1) * 4 ; i < hex_len; ++i, j -= 4) {
        rc[i] = digits[(data >> j) & 0x0F];
    }

    return rc;
}

/**
 * @func   Convert
 * @brief  None
 * @param  None
 * @retval None
 */
template<size_t N>
inline String
Convert(
    char const (&data)[N]
) {
    return String(data, std::find(data, data + N, '\0'));
}

/**
 * @func   CRC16
 * @brief  None
 * @param  None
 * @retval None
 */
inline u16_t
CRC16(
    const u8_p  pbData,
    const u32_t dwLen
) {
    u8_p pbTemp = pbData;
    u8_t byTemp;
    u16_t wCrc   = 0xF6AF; // 0x1D0F with first byte 0x56;
    u32_t length = dwLen;

    while (length--) {
        byTemp = (wCrc >> 8) ^ *pbTemp++;
        byTemp ^= byTemp >> 4;
        wCrc = (wCrc << 8) ^ ((u16_t)(byTemp << 12)) ^
               ((u16_t)(byTemp << 5)) ^ ((u16_t)byTemp);
    }

    return wCrc;
}

/******************************************************************************/
/*                             EXPORTED METHODS                               */
/******************************************************************************/

#endif /* !HELPER_HC_HPP_ */
