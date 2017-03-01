/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueStrDb.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-29 19:25:00 (Sat, 29 Oct 2016)
 *
 ******************************************************************************/

#include "Libraries/LogPlus.hpp"
#include "Libraries/LibDb/ValueStrDb.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueStrDb::ValueStrDb(
    String strValue,
    String strColumnName
) : ValueDb     (strColumnName, Value::Type_t::type_string, __FUNCTION__),
    m_strValue  (strValue) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueStrDb::ValueStrDb(
    const ValueStrDb& copied
) : ValueDb     (copied),
    m_strValue  (copied.m_strValue) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueStrDb::~ValueStrDb() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
ValueStrDb::GetValue() const {
    return m_strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueStrDb::SetValue(
    String strValue
) {
    m_strValue = strValue;
    SetChange();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueStrDb&
ValueStrDb::operator= (
    const ValueStrDb& copied
) {
    ValueStrDb temp(copied);
    Swap(temp);
    SetChange();
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueStrDb&
ValueStrDb::operator= (
    const String strValue
) {
    m_strValue = strValue;
    SetChange();
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueStrDb&
ValueStrDb::operator= (
   const_char_p rhs
) {
    m_strValue = rhs;
    SetChange();
    return *this;

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator== (
    const ValueStrDb& rhs
) const {
    return (m_strValue == rhs.m_strValue) &&
           (GetColumnName() == rhs.GetColumnName()) &&
           (GetForeignKeyName() == rhs.GetForeignKeyName()) &&
           (GetForeignKeyTable() == rhs.GetForeignKeyTable()) &&
           (GetFlag() == rhs.GetFlag());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator== (
    const String strValue
) const {
    return m_strValue == strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator!= (
    const ValueStrDb& rhs
) const {
    return m_strValue != rhs.m_strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator!= (
    const String strValue
) const {
    return m_strValue != strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator>  (
    const ValueStrDb& rhs
) const {
    return m_strValue > rhs.m_strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator>  (
    const String strValue
) const {
    return m_strValue > strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator>= (
    const ValueStrDb& rhs
) const {
    return m_strValue >= rhs.m_strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator>= (
    const String strValue
) const {
    return m_strValue >= strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator<  (
    const ValueStrDb& rhs
) const {
    return m_strValue < rhs.m_strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator<  (
    const String strValue
) const {
    return m_strValue < strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator<= (
    const ValueStrDb& rhs
) const {
    return m_strValue <= rhs.m_strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::operator<= (
    const String strValue
) const {
    return m_strValue <= strValue;
}

/**
 * @func   Swap
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueStrDb::Swap(
    ValueStrDb& other
) {
    ValueDb::Swap(other);
    std::swap(m_strValue, other.m_strValue);
}

/**
 * @func   SetValueDefault
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueStrDb::SetValueDefault() {
    m_strValue = String();
}
