#include "ValueDword.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDword::ValueDword(
    u32_t dwValue
) : Value (type_word) {
    m_dwValue = dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDword::~ValueDword() {}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
ValueDword::GetValue() const {
    return m_dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueDword::SetValue(
    u32_t dwValue
) {
    m_dwValue = dwValue;
    SetChange();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDword&
ValueDword::operator= (
    ValueDword& rhs
) {
    m_dwValue = rhs.GetValue();
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDword&
ValueDword::operator= (
    u32_t dwValue
) {
    SetValue(dwValue);
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator== (
    const ValueDword& rhs
) const {
    return m_dwValue == rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator== (
    const u32_t dwValue
) const {
    return m_dwValue == dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator!= (
    const ValueDword& rhs
) const {
    return m_dwValue != rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator!= (
    const u32_t dwValue
) const {
    return m_dwValue != dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator>  (
    const ValueDword& rhs
) const {
    return m_dwValue > rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator>  (
    const u32_t dwValue
) const {
    return m_dwValue > dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator>= (
    const ValueDword& rhs
) const {
    return m_dwValue >= rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator>= (
    const u32_t dwValue
) const {
    return m_dwValue >= dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator<  (
    const ValueDword& rhs
) const {
    return m_dwValue < rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator<  (
    const u32_t dwValue
) const {
    return m_dwValue < dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator<= (
    const ValueDword& rhs
) const {
    return m_dwValue <= rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator<= (
    const u32_t dwValue
) const {
    return m_dwValue <= dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator!= (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs != rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator== (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs == rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator>  (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs > rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator>= (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs >= rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator<  (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs < rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator<= (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs <= rhs.GetValue();
}
