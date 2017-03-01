/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueIntDb.cpp
 *
 * Author: TrungTQb
 *
 * Last Changed By:  Author: TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-07 16:10:00 (Fri, 07 Oct 2016)
 *
 ******************************************************************************/

#include "Libraries/LogPlus.hpp"
#include "Libraries/LibDb/ValueIntDb.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb::ValueIntDb(
    int_t  iValue,
    String strColumnName
) : ValueDb     (strColumnName, Value::Type_t::type_interger, __FUNCTION__),
    m_iValue    (iValue ) {

}

/**
 * @func   ValueIntDb
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb:: ValueIntDb(
    const ValueIntDb& copied
) :  ValueDb (copied),
     m_iValue (copied.m_iValue) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb::~ValueIntDb() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
ValueIntDb::GetValue(
) const {
    return m_iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueIntDb::SetValue(
    int_t iValue
) {
    m_iValue = iValue;
    SetChange();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb&
ValueIntDb::operator= (
    const ValueIntDb& copied
) {
    ValueIntDb temp(copied);
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
ValueIntDb&
ValueIntDb::operator= (
    int_t iValue
) {
    m_iValue = iValue;
    SetChange();
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb&
ValueIntDb::operator= (
    u8_t byValue
) {
    m_iValue = (int_t) byValue;
    SetChange();
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb&
ValueIntDb::operator= (
    u16_t wValue
) {
    m_iValue = (int_t) wValue;
    SetChange();
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb&
ValueIntDb::operator= (
    u32_t dwValue
) {
    m_iValue = (int_t) dwValue;
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
ValueIntDb::operator== (
    const ValueIntDb& rhs
) const {
    return (m_iValue             == rhs.m_iValue            ) &&
           (m_strColumnName      == rhs.m_strColumnName     ) &&
           (m_strForeignKeyName  == rhs.m_strForeignKeyName ) &&
           (m_strForeignKeyTable == rhs.m_strForeignKeyTable) &&
           (m_dwFlags            == rhs.m_dwFlags           );
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator== (
    const int_t iValue
) const {
    return m_iValue == iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator!= (
    const ValueIntDb& rhs
) const {
    return m_iValue != rhs.m_iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator!= (
    const int_t iValue
) const {
    return m_iValue != iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator>  (
    const ValueIntDb& rhs
) const {
    return m_iValue > rhs.m_iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator>  (
    const int_t iValue
) const {
    return m_iValue > iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator>= (
    const ValueIntDb& rhs
) const {
    return m_iValue >= rhs.m_iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator>= (
    const int_t iValue
) const {
    return m_iValue >= iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator<  (
    const ValueIntDb& rhs
) const {
    return m_iValue < rhs.m_iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator<  (
    const int_t iValue
) const {
    return m_iValue < iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator<= (
    const ValueIntDb& rhs
) const {
    return m_iValue <= rhs.m_iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator<= (
    const int_t iValue
) const {
    return m_iValue <= iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb&
ValueIntDb::operator++ () {
    ++m_iValue;
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb&
ValueIntDb::operator++ (int_t) {
    m_iValue++;
    return *this;
}

/**
 * @func   Swap
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueIntDb::Swap(
    ValueIntDb& other
) {
    ValueDb::Swap(other);
    std::swap(m_iValue, other.m_iValue);
}

/**
 * @func   SetValueDefault
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueIntDb::SetValueDefault() {
    m_iValue = -1;
}
