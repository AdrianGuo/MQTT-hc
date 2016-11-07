#include "ValueInt.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueInt::ValueInt(
    int_t iValue
) : Value (ValueType_Integer) {
     m_iValue = iValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueInt::~ValueInt() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
ValueInt::GetValue() const {
    return m_iValue;
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueInt::SetValue(
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
ValueInt&
ValueInt::operator= (
    ValueInt& rhs
) {
    m_iValue = rhs.GetValue();
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueInt&
ValueInt::operator= (
    int_t iValue
) {
    SetValue(iValue);
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator== (
    ValueInt& rhs
) const {
    return m_iValue == rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator== (
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
ValueInt::operator!= (
    ValueInt& rhs
) const {
    return m_iValue != rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator!= (
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
ValueInt::operator>  (
    ValueInt& rhs
) const {
    return m_iValue > rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator>  (
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
ValueInt::operator>= (
    ValueInt& rhs
) const {
    return m_iValue >= rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator>= (
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
ValueInt::operator<  (
    ValueInt& rhs
) const {
    return m_iValue < rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator<  (
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
ValueInt::operator<= (
    ValueInt& rhs
) const {
    return m_iValue <= rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator<= (
    const int_t iValue
) const {
    return m_iValue < iValue;
}
