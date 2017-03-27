/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Database.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 26 Dec 2016 22:00:35
 *
 ******************************************************************************/

#include "../LogPlus.hpp"
#include "Database.hpp"

/**
 * @func   Database
 * @brief  None
 * @param  None
 * @retval None
 */
Database::Database(
    const String& strConnectionString
) : m_pDtbase (NULL),
    m_strPath (strConnectionString) {
    int_t rc = sqlite3_open_v2(m_strPath.c_str(), &m_pDtbase,
               SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);

    if (rc != SQLITE_OK) {
        LOG_ERROR("failed to connect database <<%s>>:<<%s>>",
        m_strPath.c_str(), sqlite3_errmsg(m_pDtbase));
    }
}

/**
 * @func   Database
 * @brief  None
 * @param  None
 * @retval None
 */
Database::Database(
    const Database& copied
) : m_pDtbase (copied.m_pDtbase),
    m_strPath (copied.m_strPath) {

}

/**
 * @func   ~Database
 * @brief  None
 * @param  None
 * @retval None
 */
Database::~Database(
) {
    sqlite3_close_v2(m_pDtbase);
}

/**
 * @func   Connection
 * @brief  None
 * @param  None
 * @retval None
 */
sqlite3* 
Database::Connection(
) const {
    return m_pDtbase;
}

/**
 * @func   ConnectionString
 * @brief  None
 * @param  None
 * @retval None
 */
String
Database::ConnectionString(
) const {
    return m_strPath;
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
Database&
Database::operator= (
    const Database& copied
) {
    if (m_pDtbase != copied.m_pDtbase) {
        sqlite3_close_v2(m_pDtbase);
        m_pDtbase = copied.m_pDtbase;
        m_strPath = copied.m_strPath;
    }

    return *this;
}
