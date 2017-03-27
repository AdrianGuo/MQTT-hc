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

#include "../LogPlus.hpp"
#include "ValueStrDb.hpp"

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
 * @func   Compare
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueStrDb::Compare(
    String strValue,
    ValueDb::Compare typeCompare
) {
    bool_t boRet = FALSE;
    switch (typeCompare) {
    case ValueDb::Compare::Equal:
        boRet = (*this == strValue);
        break;
    case ValueDb::Compare::NotEqual:
        boRet = (*this != strValue);
        break;
    case ValueDb::Compare::Greater:
        boRet = (*this > strValue);
        break;
    case ValueDb::Compare::Less:
        boRet = (*this < strValue);
        break;
    case ValueDb::Compare::GreaterAndEqual:
        boRet = (*this >= strValue);
        break;
    case ValueDb::Compare::LessAndEqual:
        boRet = (*this <= strValue);
        break;
    }
    return boRet;
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
    return ((m_strValue           == rhs.m_strValue          ) &&
            (m_strColumnName      == rhs.m_strColumnName     ) &&
            (m_strForeignKeyName  == rhs.m_strForeignKeyName ) &&
            (m_strForeignKeyTable == rhs.m_strForeignKeyTable) &&
            (m_dwFlags            == rhs.m_dwFlags           ));
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

/**
 * @func   operator String
 * @brief  None
 * @param  None
 * @retval None
 */
ValueStrDb::operator String(
) const {
    return m_strValue;
}

/**
 * @func   operator String*
 * @brief  None
 * @param  None
 * @retval None
 */
ValueStrDb::operator String*() {
    return &m_strValue;
}
