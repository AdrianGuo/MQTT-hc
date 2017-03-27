/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: LogPlus.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 5 Dec 2016 20:34:23
 *
 ******************************************************************************/
#ifndef LOGP_LUS_HPP_
#define LOGP_LUS_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "String.hpp"
#include "Typedefs.h"
#include "LogImpl.hpp"
#include "Log.hpp"

#define __FILENAME__    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/**
 * @func   PrintArgs
 * @brief  None
 * @param  None
 * @retval None
 */
inline String
PrintArgs(
    const_char_p format, ...
) {
    char buffer[1024] = { 0 };
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    String strRet (buffer);
    return strRet;
}

/**
 * @func   MacroLog
 * @brief  None
 * @param  None
 * @retval None
 */
inline void_t
MacroLog(
    Log::Level log_level,
    const_char_p filename,
    int_t line,
    String message
) {
    char buffer[1024] = { 0 };
    snprintf(buffer, sizeof(buffer), "[%18s:%4d] %s", filename, line, message.c_str());
    String strInfo = buffer;
    Log::Write(log_level, strInfo.c_str());
}

#define LOG_CRITICAL(...) \
    ST(\
        String varlist = PrintArgs(__VA_ARGS__); \
        MacroLog(Log::Level::eCritic, __FILENAME__, __LINE__, varlist); \
    )

#define LOG_ERROR(...) \
    ST(\
        String varlist = PrintArgs(__VA_ARGS__); \
        MacroLog(Log::Level::eError, __FILENAME__, __LINE__, varlist); \
    )

#define LOG_WARN(...) \
    ST(\
        String varlist = PrintArgs(__VA_ARGS__); \
        MacroLog(Log::Level::eWarn, __FILENAME__, __LINE__, varlist); \
    )

#define LOG_INFO(...) \
    ST(\
        String varlist = PrintArgs(__VA_ARGS__); \
        MacroLog(Log::Level::eInfo, __FILENAME__, __LINE__, varlist); \
    )

#define LOG_DEBUG(...) \
    ST(\
        String varlist = PrintArgs(__VA_ARGS__); \
        MacroLog(Log::Level::eDebug, __FILENAME__, __LINE__, varlist); \
    )

#endif /* !LOGP_LUS_HPP_ */
