#include "ValueWord.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueWord::ValueWord(
    u16_t wValue
) : Value (type_word) {
    m_wValue = wValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueWord::~ValueWord() {}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u16_t
ValueWord::GetValue() const {
    return m_wValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueWord::SetValue(
    u16_t wValue
) {
    m_wValue = wValue;
    SetChange();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueWord&
ValueWord::operator= (
    ValueWord& rhs
) {
    m_wValue = rhs.GetValue();
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueWord&
ValueWord::operator= (
    u16_t wValue
) {
    SetValue(wValue);
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator== (
    const ValueWord& rhs
) const {
    return m_wValue == rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator== (
    const u16_t wValue
) const {
    return m_wValue == wValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator!= (
    const ValueWord& rhs
) const {
    return m_wValue != rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator!= (
    const u16_t wValue
) const {
    return m_wValue != wValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator> (
    const ValueWord& rhs
) const {
    return m_wValue > rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator> (
    const u16_t wValue
) const {
    return m_wValue > wValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator>= (
    const ValueWord& rhs
) const {
    return m_wValue >= rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator>= (
    const u16_t wValue
) const {
    return m_wValue >= wValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator< (
    const ValueWord& rhs
) const {
    return m_wValue < rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator< (
    const u16_t wValue
) const {
    return m_wValue < wValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator<= (
    const ValueWord& rhs
) const {
    return m_wValue < rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueWord::operator<= (
    const u16_t wValue
) const {
    return m_wValue <= wValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator!= (
    const u16_t lhs,
    const ValueWord& rhs
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
    const u16_t lhs,
    const ValueWord& rhs
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
    const u16_t lhs,
    const ValueWord& rhs
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
    const u16_t lhs,
    const ValueWord& rhs
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
    const u16_t lhs,
    const ValueWord& rhs
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
    const u16_t lhs,
    const ValueWord& rhs
) {
    return lhs <= rhs.GetValue();
}
