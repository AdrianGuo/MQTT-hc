#include "ValueByte.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueByte::ValueByte(
    u8_t byValue
) : Value (ValueType_Byte) {
    m_byValue = byValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueByte::~ValueByte() {}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ValueByte::GetValue() const {
    return m_byValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueByte::SetValue(
    const u8_t byValue
) {
    m_byValue = byValue;
    SetChange();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueByte&
ValueByte::operator= (
    ValueByte& rhs
) {
    m_byValue = rhs.GetValue();
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueByte&
ValueByte::operator= (
    const u8_t byValue
) {
    SetValue(byValue);
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator== (
    ValueByte& rhs
) const {
    return m_byValue == rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator== (
    const u8_t byValue
) const {
    return m_byValue == byValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator!= (
    ValueByte& rhs
) const {
    return m_byValue != rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator!= (
    const u8_t byValue
) const {
    return m_byValue != byValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator>  (
    ValueByte& rhs
) const {
    return m_byValue > rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator>  (
    const u8_t byValue
) const {
    return m_byValue > byValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator>= (
    ValueByte& rhs
) const {
    return m_byValue >= rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator>= (
    const u8_t byValue
) const {
    return m_byValue >= byValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator<  (
    ValueByte& rhs
) const {
    return m_byValue < rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator<  (
    const u8_t byValue
) const {
    return m_byValue < byValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator<= (
    ValueByte& rhs
) const {
    return m_byValue <= rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueByte::operator<= (
    const u8_t byValue
) const {
    return m_byValue <= byValue;
}
