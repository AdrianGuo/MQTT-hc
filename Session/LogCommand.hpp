/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: LogCommand.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 26 Dec 2016 22:05:19
 *
 ******************************************************************************/
#ifndef LOG_COMMAND_HPP_
#define LOG_COMMAND_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "LogPlus.hpp"
#include "JsonCommand.hpp"

#ifndef SPACE
#define SPACE                              (' ')
#endif /* SPACE */

#ifndef ENDLN
#define ENDLN                              ('\n')
#endif /* ENDLN */

#ifndef AT
#define AT                                 ('@')
#endif /* AT */

#define LOG_COMMAND(logLevel, jsonCommand) \
    ST(\
        const String STA = String("$"); \
        const String END = String("$end"); \
        String strRecevCmd = STA + (jsonCommand)->GetFullCommand() + (jsonCommand)->GetJsonValue() + END;\
        strRecevCmd.remove_char(ENDLN); \
        strRecevCmd.remove_char(SPACE); \
        std::replace(strRecevCmd.begin(), strRecevCmd.end(), AT, SPACE); \
        if ((logLevel) == Log::Level::eInfo) { \
            LOG_INFO (" json %s", strRecevCmd.c_str()); \
        } else if ((logLevel) == Log::Level::eDebug) { \
            LOG_DEBUG(" json %s", strRecevCmd.c_str()); \
        } \
    )

#endif /* LOG_COMMAND_HPP_ */
