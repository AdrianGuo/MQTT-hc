/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueDb.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2 Feb 2017 21:00:04
 *
 ******************************************************************************/

#include "Libraries/LogPlus.hpp"
#include "Libraries/LibDb/ValueDb.hpp"

/**
 * @func   ValueDb
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDb::ValueDb (
    const String    strColumnName,
    Value::Type_t   valueType,
    const_char_p    pName,
    const String    strForeignKeyTable,
    const String    strForeignKeyName,
    u32_t           dwFlags
) : ValueDbCore         (valueType, pName   ),
    m_dwFlags           (dwFlags            ),
    m_strColumnName     (strColumnName      ),
    m_strForeignKeyTable(strForeignKeyTable ),
    m_strForeignKeyName (strForeignKeyName  ) {
    if (valueType == Value::Type_t::type_interger) {
        m_strSqlType = "INTEGER";
    } else if (valueType == Value::Type_t::type_string) {
        m_strSqlType = "TEXT";
    } else if (valueType == Value::Type_t::type_decimal) {
        m_strSqlType = "REAL";
    }

    #ifdef DB_VALUE_DB
    LOG_DEBUG("[%18s] object [%03d] created ", m_strColumnName.c_str(), Index());
    #endif /* DB_VALUE_DB */
}

/**
 * @func   ValueDb
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDb::ValueDb(
    const ValueDb& copied
) : ValueDbCore          (copied                     ),
    m_dwFlags            (copied.m_dwFlags           ),
    m_strColumnName      (copied.m_strColumnName     ),
    m_strSqlType         (copied.m_strSqlType        ),
    m_strForeignKeyTable (copied.m_strForeignKeyTable),
    m_strForeignKeyName  (copied.m_strForeignKeyName ) {

}

/**
 * @func   ValueDb
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDb::~ValueDb() {
    #ifdef DB_VALUE_DB
    LOG_DEBUG("[%18s] object [%05d] deleted ", m_strColumnName.c_str(), Index());
    #endif /* DB_VALUE_DB */
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDb&
ValueDb::operator= (
    const ValueDb& copied
) {
    ValueDb temp(copied);
    Swap(temp);
    return *this;
}

/**
 * @func   Swap
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::Swap(
    ValueDb& other
) {
    ValueDbCore::Swap(other);
    std::swap(m_strColumnName,      other.m_strColumnName       );
    std::swap(m_strForeignKeyTable, other.m_strForeignKeyTable  );
    std::swap(m_strForeignKeyName,  other.m_strForeignKeyName   );
    std::swap(m_dwFlags,            other.m_dwFlags             );
    std::swap(m_strSqlType,         other.m_strSqlType          );
}

/**
 * @func   SetColumnName
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetColumnName(
    const String strColumn
) {
    m_strColumnName = strColumn;
}

/**
 * @func   SetForeignKeyTable
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetForeignKeyTable(
    const String strForeignKeyTable
) {
    m_strForeignKeyTable = strForeignKeyTable;
}

/**
 * @func   SetForeignKeyName
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetForeignKeyName(
    const String strForeignKeyName
) {
    m_strForeignKeyName = strForeignKeyName;
}

/**
 * @func   SetFlag
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetFlag(
    u32_t dwFlags
) {
    m_dwFlags |= dwFlags;
}

/**
 * @func   GetColumnName
 * @brief  None
 * @param  None
 * @retval None
 */
String
ValueDb::GetColumnName(
) const {
    return m_strColumnName;
}

/**
 * @func   GetForeignKeyTable
 * @brief  None
 * @param  None
 * @retval None
 */
String
ValueDb::GetForeignKeyTable(
) const {
    return m_strForeignKeyTable;
}

/**
 * @func   GetForeignKeyName
 * @brief  None
 * @param  None
 * @retval None
 */
String
ValueDb::GetForeignKeyName(
) const {
    return m_strForeignKeyName;
}

/**
 * @func   GetFlag
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
ValueDb::GetFlag(
) const {
    return m_dwFlags;
}

/**
 * @func   GetSqlType
 * @brief  None
 * @param  None
 * @retval None
 */
String
ValueDb::GetSqlType(
) const {
    return m_strSqlType;
}

/**
 * @func   SetSqlType
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetSqlType(
     String strSqlType
) {
    m_strSqlType = strSqlType;
}

/**
 * @func   SetNotNull
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetNotNull() {
    m_dwFlags |= NotNull;
}

/**
 * @func   SetInsteadId
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetInsteadId() {
    m_dwFlags |= InsteadId;
}

/**
 * @func   SetNaturalId
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetNaturalId() {
    m_dwFlags |= NaturalId;
}

/**
 * @func   SetForeignKey
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetForeignKey() {
    m_dwFlags |= ForeignKey;
}

/**
 * @func   SetBindNull
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetBindNull() {
    m_dwFlags |= BindNull;
}

/**
 * @func   ResetNotNull
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::ResetNotNull() {
    m_dwFlags &= ~NotNull;
}

/**
 * @func   ResetInsteadId
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::ResetInsteadId() {
    m_dwFlags &= ~InsteadId;
}

/**
 * @func   ResetNaturalId
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::ResetNaturalId() {
    m_dwFlags &= ~NaturalId;
}

/**
 * @func   ResetForeignKey
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::ResetForeignKey() {
    m_dwFlags &= ~ForeignKey;
}

/**
 * @func   ResetBindNull
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::ResetBindNull() {
    m_dwFlags &= ~BindNull;
}

/**
 * @func   IsNotNull
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDb::IsNotNull(
) const {
    return (m_dwFlags & NotNull) != 0;
}

/**
 * @func   IsInsteadId
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDb::IsInsteadId(
) const {
    return (m_dwFlags & InsteadId) != 0;
}

/**
 * @func   IsNaturalId
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDb::IsNaturalId(
) const {
    return (m_dwFlags & NaturalId) != 0;
}

/**
 * @func   IsIdField
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDb::IsIdField(
) const {
    return (m_dwFlags & (InsteadId | NaturalId)) != 0;
}

/**
 * @func   IsForeignkey
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDb::IsForeignkey(
) const {
    return (m_dwFlags & ForeignKey) != 0;
}

/**
 * @func   IsBindNull
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDb::IsBindNull(
) const {
    return (m_dwFlags & BindNull) != 0;
}

/**
 * @func   SetOnDeleteCascade
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetOnDeleteCascade() {
    m_dwFlags |= OnDeleteCascade;
}

/**
 * @func   SetOnDeleteSetNull
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetOnDeleteSetNull() {
    m_dwFlags |= OnDeleteSetNull;
}

/**
 * @func   SetOnUpdateCascade
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetOnUpdateCascade() {
    m_dwFlags |= OnUpdateCascade;
}

/**
 * @func   SetOnUpdateSetNull
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::SetOnUpdateSetNull() {
    m_dwFlags |= OnUpdateSetNull;
}


/**
 * @func   ResetOnDeleteCascade
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::ResetOnDeleteCascade() {
    m_dwFlags &= ~OnDeleteCascade;
}

/**
 * @func   ResetOnDeleteSetNull
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::ResetOnDeleteSetNull() {
    m_dwFlags &= ~OnDeleteSetNull;
}

/**
 * @func   ResetOnUpdateCascade
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::ResetOnUpdateCascade() {
    m_dwFlags &= ~OnUpdateCascade;
}

/**
 * @func   ResetOnUpdateSetNull
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDb::ResetOnUpdateSetNull() {
    m_dwFlags &= ~OnUpdateSetNull;
}

/**
 * @func   IsOnDeleteCascade
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDb::IsOnDeleteCascade(
) const {
    return (m_dwFlags & OnDeleteCascade) != 0;
}

/**
 * @func   IsOnDeleteSetNull
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDb::IsOnDeleteSetNull(
) const {
    return (m_dwFlags & OnDeleteSetNull) != 0;
}

/**
 * @func   IsOnUpdateCascade
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDb::IsOnUpdateCascade(
) const {
    return (m_dwFlags & OnUpdateCascade) != 0;
}

/**
 * @func   IsOnUpdateSetNull
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDb::IsOnUpdateSetNull(
) const {
    return (m_dwFlags & OnUpdateSetNull) != 0;
}
