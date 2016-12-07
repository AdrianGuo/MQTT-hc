/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Log.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 10:17:07
 *
 ******************************************************************************/
#include <stdarg.h>
#include "LogImpl.hpp"
#include "Log.hpp"

Log_p Log::m_pInstance = NULL;
LogImpl_p Log::m_pLogImpl = NULL;

const_char_p LogLevelString[] = {
    "[CRIT]",
    "[EROR]",
    "[WARN]",
    "[INFO]",
    "[DBUG]",
    "[NOTE]"
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Log::Log(
    const String& strFileName,
    const bool_t  boAppend,
    const bool_t  boConsole,
    const Level   logFile,
    const Level   logQueue
) : m_pLocker (new Locker()) {
    if (m_pLogImpl == NULL) {
        m_pLogImpl = new LogImpl(strFileName, boAppend, boConsole, logFile, logQueue);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Log::Release() {
    if (m_pInstance != NULL) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

/**
 * @func   Create
 * @brief  None
 * @param  None
 * @retval None
 */
Log_p
Log::Create(
    const String& strFileName,
    const bool_t  boAppend,
    const bool_t  boConsole,
    const Level   levellogFile,
    const Level   levellogQueue
) {
    if (m_pInstance == NULL) {
        m_pInstance = new Log(strFileName, boAppend, boConsole, levellogFile, levellogQueue);
    } else {
        Log::Release();
        m_pInstance = new Log(strFileName, boAppend, boConsole, levellogFile, levellogQueue);
    }

    return m_pInstance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Log::~Log() {
    if (m_pLogImpl != NULL) {
        delete m_pInstance->m_pLocker;
        delete m_pLogImpl;
        m_pLogImpl = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Log::Write(
    Level level,
    const_char_p format, ...
) {
    if ((m_pLogImpl != NULL) && (m_pInstance != NULL)) {
        m_pInstance->m_pLocker->Lock();
        va_list args;
        va_start(args, format);
        m_pInstance->m_pLogImpl->Write(level, format, args);
        va_end(args);
        m_pInstance->m_pLocker->UnLock();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Log::Queue(
    String strOut
) {
    if (m_pLogImpl != NULL && m_pInstance != NULL) {
        m_pInstance->m_pLocker->Lock();
        m_pLogImpl->Queue(strOut);
        m_pInstance->m_pLocker->UnLock();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Log::PushQueue() {
    if (m_pLogImpl != NULL && m_pInstance != NULL) {
        m_pInstance->m_pLocker->Lock();
        m_pLogImpl->PushQueue();
        m_pInstance->m_pLocker->UnLock();
    }
}
