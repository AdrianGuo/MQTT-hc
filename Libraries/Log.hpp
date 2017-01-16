/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Log.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 10:01:37
 *
 ******************************************************************************/
#ifndef LOG_HPP_
#define LOG_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "List.hpp"
#include "Locker.hpp"

class LogImpl;

extern const_char_p LogLevelString[];

class Log {
public:
    typedef enum {
        eCritic = 0,
        eError,
        eWarn,
        eInfo,
        eDebug,
        eAll
    } Level;

    static Log* Create(const String& strFileName,
                       const bool_t  boAppend,
                       const bool_t  boConsole,
                       const Level   levellogFile,
                       const Level   levellogQueue);

    static void_t Write(Level level, const_char_p format, ...);
    static void_t Release();
    static void_t Queue(String strOut);
    static void_t PushQueue();
private:
    Locker_p m_pLocker;
    static Log*     m_pInstance;
    static LogImpl* m_pLogImpl;

    Log(const String& strFileName,
        const bool_t  boAppend,
        const bool_t  boConsole,
        const Level   logFile,
        const Level   logQueue);
    ~Log();
};

typedef Log  Log_t;
typedef Log* Log_p;

#endif /* LIBRARIES_LOG_HPP_ */
