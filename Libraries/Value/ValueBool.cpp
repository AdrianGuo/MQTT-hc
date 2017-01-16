#include "ValueBool.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueBool::ValueBool(
    bool_t boValue
) : Value (type_bool) {
    m_boValue = boValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueBool::~ValueBool() {}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueBool::GetValue() const {
    return m_boValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueBool::SetValue(
    bool_t boValue
) {
    m_boValue = boValue;
    SetChange();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueBool&
ValueBool::operator= (
    bool_t boValue
) {
    SetValue(boValue);
    return *this;
}

/**
 * @func   operator==
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueBool::operator== (
    const ValueBool& rhs
) const {
    return m_boValue == rhs.GetValue();
}

/**
 * @func   operator!=
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueBool::operator!= (
    const ValueBool& rhs
) const {
    return m_boValue != rhs.GetValue();
}

/**
 * @func   operator==
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueBool::operator== (
    const bool_t boValue
) const {
    return m_boValue == boValue;
}

/**
 * @func   operator!=
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueBool::operator!= (
    const bool_t boValue
) const {
    return m_boValue != boValue;
}

/**
 * @func   operator!=
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator!= (
    const bool_t lhs,
    const ValueBool& rhs
) {
    return lhs != rhs.GetValue();
}

/**
 * @func   operator==
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator== (
    const bool_t lhs,
    const ValueBool& rhs
) {
    return lhs == rhs.GetValue();
}
