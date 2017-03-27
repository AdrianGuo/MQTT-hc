/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: LogImpl.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 10:24:52
 *
 ******************************************************************************/
#ifndef LOGIMPL_HPP_
#define LOGIMPL_HPP_

#include "Typedefs.h"
#include "String.hpp"
#include "List.hpp"
#include "Log.hpp"

class LogImpl {
private:
    String GetTime();
    String GetThreadId ();
    String GetLogLevel (Log::Level level);
    u32_t  ToEscapeCode(Log::Level level);

    List<String> m_queLog;
    String m_strFileName;
    bool_t m_boAppend;
    bool_t m_boConsole;
    Log::Level m_logFile;
    Log::Level m_logQueue;
    FILE*  m_pFile;
public:
    static const u32_t queuemax = 500;

    LogImpl(const String& filename,
            const bool_t  boAppend,
            const bool_t  boConsole,
            const Log::Level logFile,
            const Log::Level logQueue);

    ~LogImpl();

    void_t Write(Log::Level level, const_char_p format, va_list args);
    void_t Queue(String strOut);
    void_t PushQueue();
};

typedef LogImpl  LogImpl_t;
typedef LogImpl* LogImpl_p;

#endif /* LIBRARIES_LOGIMPL_HPP_ */
