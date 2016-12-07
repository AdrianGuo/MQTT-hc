/*******************************************************************************
*
* Copyright (c) 2016
* Lumi, JSC.
* All Rights Reserved
*
*
* Description:      Include file for application
*
* Author:
*
* Last Changed By:  TrungTQ
* Revision:         Revision: 1.0
* Last Changed:     Date: 2016-08-05 10:15:00 (Fri, 04 Aug 2016)
*
* Note:             This file base on lib of ivanseidel
*                   URL: https://github.com/ivanseidel/LinkedList
*
******************************************************************************/
#ifndef CONVERT_HPP_
#define CONVERT_HPP_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "typedefs.h"
#include "String.hpp"
#include "JsonCommand.hpp"
#include "LogPlus.hpp"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifndef SPACE
#define SPACE                              (' ')
#endif /* SPACE */

#ifndef ENDLN
#define ENDLN                              ('\n')
#endif /* ENDLN */

#ifndef AT
#define AT                                 ('@')
#endif /* AT */

#define LOGCOMMAND(logLevel, jsonCommand) \
    ST(\
        const String STA = String("$"); \
        const String END = String("$end"); \
        String strRecevCmd  = STA + (jsonCommand)->GetFullCommand() + (jsonCommand)->GetJsonValue() + END;\
        strRecevCmd.remove_char(ENDLN); \
        strRecevCmd.remove_char(SPACE); \
        std::replace(strRecevCmd.begin(), strRecevCmd.end(), AT, SPACE); \
        if ((logLevel) == Log::Level::eInfo) { \
            LOG_INFO("jsoncmd %s", strRecevCmd.c_str()); \
        } else if ((logLevel) == Log::Level::eDebug) { \
            LOG_DEBUG("jsoncmd %s", strRecevCmd.c_str()); \
        } \
    )

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
    for (size_t i = 0, j = (hex_len - 1) * 4 ; i < hex_len; ++i, j -= 4)
        rc[i] = digits[(data >> j) & 0x0F];

    return rc;
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

/**
 * @func   CopyFile
 * @brief  None
 * @param  None
 * @retval None
 */
inline void_t
CopyFile(
    const_char_p srcFile,
    const_char_p desFile
) {
    int_t read_fd;
    int_t write_fd;
    struct stat stat_buf;
    off_t offset = 0;
    read_fd = open(srcFile, O_RDONLY);
    if (read_fd == -1) { return; }
    fstat (read_fd, &stat_buf);
    write_fd = open(desFile, O_WRONLY | O_CREAT, stat_buf.st_mode);
    if (write_fd == -1) { return; }
    sendfile (write_fd, read_fd, &offset, stat_buf.st_size);
    close (read_fd);
    close (write_fd);
}

/******************************************************************************/
/*                             EXPORTED METHODS                               */
/******************************************************************************/


#endif /* CONVERT_HPP_ */
