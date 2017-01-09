/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: LogImpl.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 10:25:25
 *
 ******************************************************************************/
#include <stdio.h>
#include <sys/time.h>
#include <stdarg.h>
#include "debug.hpp"
#include "LogImpl.hpp"

/**
 * @func   LogImpl
 * @brief  None
 * @param  None
 * @retval None
 */
LogImpl::LogImpl(
    const String& filename,
    const bool_t  boAppend,
    const bool_t  boConsole,
    const Log::Level logFile,
    const Log::Level logQueue
) : m_strFileName (filename),
    m_boAppend (boAppend),
    m_boConsole (boConsole),
    m_logFile (logFile),
    m_logQueue (logQueue),
    m_pFile (NULL) {
    if (!filename.empty()) {
        if (m_boAppend) {
            m_pFile = fopen(m_strFileName.c_str(), "a");
        } else {
            m_pFile = fopen(m_strFileName.c_str(), "w");
        }

        if (m_pFile == NULL) {
            ERROR("cannot open file");
        } else {
            setlinebuf(m_pFile);
        }
    }
    setlinebuf(stdout);
}

/**
 * @func   ~LogImpl
 * @brief  None
 * @param  None
 * @retval None
 */
LogImpl::~LogImpl() {
    if (m_pFile != NULL) {
        fclose(m_pFile);
    }
}

/**
 * @func   ToEscapeCode
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
LogImpl::ToEscapeCode(
    Log::Level level
) {
    u32_t dwCode;
    switch (level) {
    case Log::Level::eCritic:
        dwCode = 95;
        break;

    case Log::Level::eDebug:
        dwCode = 36;
        break;

    case Log::Level::eError:
        dwCode = 31;
        break;

    case Log::Level::eWarn:
        dwCode = 33;
        break;

    case Log::Level::eInfo:
        dwCode = 39;
        break;

    case Log::Level::eAll:
        dwCode = 32;
        break;

    default:
        dwCode = 39;
    }
    return dwCode;
}

/**
 * @func   Write
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LogImpl::Write(
    Log::Level loglevel,
    const_char_p format,
    va_list args
) {
    String strTime     = GetTime();
    String strLogLevel = GetLogLevel(loglevel);
    String strThreadId = GetThreadId();

    char lineBuffer[1024] = { 0 };

    if ((format != NULL) && (format[0] != '\0')) {
        va_list saveargs;
        va_copy(saveargs, args);
        vsnprintf(lineBuffer, sizeof(lineBuffer), format, args);
        va_end(saveargs );
    }

    if ((m_pFile != NULL) || m_boConsole) {
        String strOutBuff;
        strOutBuff.append(strLogLevel);
        strOutBuff.append(strTime);
        strOutBuff.append(strThreadId);
        strOutBuff.append(lineBuffer);
        strOutBuff.append("\n");

        if ((m_pFile != NULL) && (m_logFile >= loglevel)) {
            fputs(strOutBuff.c_str(), m_pFile);
        }

        if (m_boConsole) {
            fprintf(stdout, "\x1B[%02um", ToEscapeCode(loglevel));
            fputs(strOutBuff.c_str(), stdout);
            fprintf(stdout, "\x1b[39m");
        }
        Queue(strOutBuff);
    }
}

/**
 * @func   GetTime
 * @brief  None
 * @param  None
 * @retval None
 */
String
LogImpl::GetTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm* now;
    now = localtime(&tv.tv_sec);
    char buffer[100] = { 0 };
    snprintf(buffer, sizeof(buffer), "[%04d-%02d-%02d %02d:%02d:%02d.%03d]",
    now->tm_year + 1990, now->tm_mon + 1, now->tm_mday,
    now->tm_hour, now->tm_min, now->tm_sec, (int_t)tv.tv_usec / 1000);
    String strTime = buffer;
    return strTime;
}

/**
 * @func   GetThreadId
 * @brief  None
 * @param  None
 * @retval None
 */
String
LogImpl::GetThreadId() {
    char buffer[20] = { 0 };
    snprintf(buffer, sizeof(buffer), "[%08u]", (u32_t) pthread_self());
    String strThreadId = buffer;
    return strThreadId;
}

/**
 * @func   GetLogLevel
 * @brief  None
 * @param  None
 * @retval None
 */
String
LogImpl::GetLogLevel(
    Log::Level loglevel
) {
    if (loglevel <= Log::Level::eAll) {
        String strLogLevel = LogLevelString[loglevel];
        return strLogLevel;
    }
    return "[Unknown]";
}

/**
 * @func   Queue
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LogImpl::Queue(
    String strOut
) {
    m_queLog.push_back(strOut);
    if (m_queLog.size() > queuemax) {
        m_queLog.pop_front();
    }
}

/**
 * @func   PushQueue
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LogImpl::PushQueue() {
    List<String>::iterator_t it = m_queLog.begin();
    while (it != m_queLog.end()) {
        String strTemp = *it;
        Log::Write(Log::Level::eAll, "%s", strTemp.c_str());
        it++;
    }
    m_queLog.clear();
}
