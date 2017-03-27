/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueDbCore.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2 Feb 2017 20:56:45
 *
 ******************************************************************************/

#include "../LogPlus.hpp"
#include "ValueDbCore.hpp"

/**
 * @func   ValueDbCore
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDbCore::ValueDbCore(
    Type_t valueType,
    const_char_p pName
) : Value       (valueType, pName),
    m_boChanged (FALSE) {
}

/**
 * @func   ValueDbCore
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDbCore::ValueDbCore(
    const ValueDbCore& copied
) : Value (copied),
    m_boChanged (copied.m_boChanged) {
}


/**
 * @func   ~ValueDbCore
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDbCore::~ValueDbCore() {
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDbCore&
ValueDbCore::operator= (
    const ValueDbCore& copied
) {
    ValueDbCore temp(copied);
    Swap(temp);
    m_boChanged = TRUE;
    return *this;
}

/**
 * @func   Swap
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDbCore::Swap(
    ValueDbCore& other
) {
    Value::Swap(other);
    std::swap(m_boChanged, other.m_boChanged);
}

/**
 * @func   Swap
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDbCore::SetChange() {
    m_boChanged = TRUE;
}

/**
 * @func   ResetChange
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDbCore::ResetChange() {
    m_boChanged = FALSE;
}

/**
 * @func   IsChanged
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDbCore::IsChanged(
) const {
    return m_boChanged;
}
