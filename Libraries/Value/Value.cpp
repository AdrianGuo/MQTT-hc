/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Value.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 20 Jan 2017 10:40:07
 *
 ******************************************************************************/

#include "Libraries/LogPlus.hpp"
#include "Libraries/Value/Value.hpp"

/**
 * @func   Swap
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Value::Swap(
    Value& other
) {
    ValueCore::Swap(other);
    std::swap(m_valueType, other.m_valueType);
}

/**
 * @func   Value
 * @brief  None
 * @param  None
 * @retval None
 */
Value::Value(
    Type_t       valueType,
    const_char_p pName
) : ValueCore   (pName    ),
    m_valueType (valueType),
    m_boChanged (FALSE) {
}

/**
 * @func   Value
 * @brief  None
 * @param  None
 * @retval None
 */
Value::Value(
    const Value& copied
) : ValueCore   (copied),
    m_valueType (copied.m_valueType),
    m_boChanged (FALSE) {

}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
Value&
Value::operator= (
    const Value& copied
) {
    Value temp(copied);
    Swap(temp);
    return *this;
}

/**
 * @func   ~Value
 * @brief  None
 * @param  None
 * @retval None
 */
Value::~Value() {
}

/**
 * @func   SetType
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Value::SetType(
    Type_t valueType
) {
    m_valueType = valueType;
}

/**
 * @func   GetType
 * @brief  None
 * @param  None
 * @retval None
 */
Value::Type_t
Value::GetType() const {
    return m_valueType;
}

/**
 * @func   operator==
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Value::operator== (
    const Value& rhs
) const {
    return m_valueType == rhs.m_valueType;
}

/**
 * @func   operator!=
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Value::operator!= (
    const Value& rhs
) const {
    return m_valueType != rhs.m_valueType;
}

/**
 * @func   operator>=
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Value::operator>= (
    const Value& rhs
) const {
    return m_valueType >= rhs.m_valueType;
}

/**
 * @func   operator<=
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Value::operator<= (
    const Value& rhs
) const {
    return m_valueType <= rhs.m_valueType;
}

/**
 * @func   operator>
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Value::operator> (
    const Value& rhs
) const {
    return m_valueType > rhs.m_valueType;
}

/**
 * @func   operator<
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Value::operator< (
    const Value& rhs
) const {
    return m_valueType < rhs.m_valueType;
}
