/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-05-16 11:45:00 (Tue, 16 May 2016)
 *
 ******************************************************************************/
#include <string.h>
#include "debug.hpp"
#include "Sqlite3Statement.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Sqlite3Statement::Sqlite3Statement(
    Database_p pDatabase,
    const String& strSql
) : m_pDatabase (pDatabase),
    m_strSql (strSql) {
    int_t rc = sqlite3_prepare_v2(m_pDatabase->Connection(), m_strSql.c_str(),
            static_cast<int> (m_strSql.length() + 1), &m_pStmt, 0);

    if (rc != SQLITE_OK) { DEBUG1("PREPARE ERROR"); }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Sqlite3Statement::~Sqlite3Statement() {
    sqlite3_finalize(m_pStmt);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    int_t column,
    i8_t  value
) {
    return sqlite3_bind_int(m_pStmt, column + 1, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    int_t column,
    i16_t value
) {
    return sqlite3_bind_int(m_pStmt, column + 1, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    int_t column,
    i32_t value
) {
    return sqlite3_bind_int(m_pStmt, column + 1, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    int_t column,
    i64_t value
) {
    return sqlite3_bind_int(m_pStmt, column + 1, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    int_t column,
    flo_t value
) {
    return sqlite3_bind_double(m_pStmt, column + 1, static_cast<dob_t> (value));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    int_t column,
    dob_t value
) {
    return sqlite3_bind_double(m_pStmt, column + 1, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    int_t column,
    const_char_p value
) {
    return sqlite3_bind_text(m_pStmt, column + 1, value, strlen(value), SQLITE_TRANSIENT);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    int_t column,
    void_p value,
    int_t n
) {
    return 0;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    int_t column,
    const String& value
) {
    return bind(column, value.c_str());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    int_t column
) {
    return sqlite3_bind_null(m_pStmt, column);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    int_t column,
    ValueIntDb value
) {
    return bind(column, value.GetValue());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    const_char_p name,
    i8_t  value
) {
    int_t column = sqlite3_bind_parameter_index(m_pStmt, name);
    return bind(column, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    const_char_p name,
    i16_t value
) {
    int_t column = sqlite3_bind_parameter_index(m_pStmt, name);
    return bind(column, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    const_char_p name,
    i32_t value
) {
    int_t column = sqlite3_bind_parameter_index(m_pStmt, name);
    return bind(column, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    const_char_p name,
    i64_t value
) {
    int_t column = sqlite3_bind_parameter_index(m_pStmt, name);
    return bind(column, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    const_char_p name,
    flo_t value
) {
    int_t column = sqlite3_bind_parameter_index(m_pStmt, name);
    return bind(column, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    const_char_p name,
    dob_t value
) {
    int_t column = sqlite3_bind_parameter_index(m_pStmt, name);
    return bind(column, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(const_char_p name, const_char_p value) {
    int_t column = sqlite3_bind_parameter_index(m_pStmt, name);
    return bind(column, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    const_char_p name,
    void_p value,
    int_t n
) {
    return 0;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    const_char_p name,
    const String& value
) {
    int_t column = sqlite3_bind_parameter_index(m_pStmt, name);
    return bind(column, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::bind(
    const_char_p name
) {
    int_t column = sqlite3_bind_parameter_index(m_pStmt, name);
    return bind(column);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Sqlite3Statement::get(
    int_t column,
    i8_p value
) {
    if (sqlite3_column_type(m_pStmt, column) == SQLITE_NULL) {
        return FALSE;
    }

    int_t iValue = 0;
    iValue = sqlite3_column_int(m_pStmt, column);
    *value = iValue;

    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Sqlite3Statement::get(
    int_t column,
    i16_p value
) {
    if (sqlite3_column_type(m_pStmt, column) == SQLITE_NULL) { return FALSE; }

    int_t iValue = 0;
    iValue = sqlite3_column_int(m_pStmt, column);
    *value = iValue;

    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Sqlite3Statement::get(
    int_t column,
    i32_p value
) {
    if (sqlite3_column_type(m_pStmt, column) == SQLITE_NULL) {
        return FALSE;
    }

    int_t iValue = 0;
    iValue = sqlite3_column_int(m_pStmt, column);
    *value = iValue;

    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Sqlite3Statement::get(
    int_t column,
    i64_p value
) {
    if (sqlite3_column_type(m_pStmt, column) == SQLITE_NULL) {
        return FALSE;
    }

    *value = sqlite3_column_int64(m_pStmt, column);

    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Sqlite3Statement::get(
    int_t column,
    flo_p value
) {
    if (sqlite3_column_type(m_pStmt, column) == SQLITE_NULL) { return FALSE; }

    *value = static_cast<flo_t> (sqlite3_column_double(m_pStmt, column));

    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Sqlite3Statement::get(
    int_t column,
    dob_p value
) {
    if (sqlite3_column_type(m_pStmt, column) == SQLITE_NULL) { return FALSE; }

    *value = sqlite3_column_double(m_pStmt, column);

    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Sqlite3Statement::get(
    int_t column,
    String* value
) {
    if (sqlite3_column_type(m_pStmt, column) == SQLITE_NULL) { return FALSE; }

    *value = (const_char_p) sqlite3_column_text(m_pStmt, column);

    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Sqlite3Statement::get(
    int_t column,
    void_p value
) {
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Sqlite3Statement::get(
    int_t column,
    ValueIntDb_p value
) {
    return get(column, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
i64_t
Sqlite3Statement::insertId() {
    return sqlite3_last_insert_rowid(m_pDatabase->Connection());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Sqlite3Statement::reset() {
    if (m_pStmt != NULL) {
        sqlite3_reset(m_pStmt);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Sqlite3Statement::ubind() {
    if (m_pStmt != NULL) {
        sqlite3_clear_bindings(m_pStmt);
    }
}

void_t
Sqlite3Statement::init() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::count() {
    return sqlite3_data_count(m_pStmt);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Sqlite3Statement::execute() {
    int_t rc = sqlite3_step(m_pStmt);
    return rc;
}
