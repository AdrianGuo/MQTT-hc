#include "ValueInt.hpp"

/**
 * @func   ValueInt
 * @brief  None
 * @param  None
 * @retval None
 */
ValueInt::ValueInt(
    int_t iValue
) : Value (type_interger) {
     m_iValue = iValue;
}

/**
 * @func   ~ValueInt
 * @brief  None
 * @param  None
 * @retval None
 */
ValueInt::~ValueInt() {
}

/**
 * @func   GetValue
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
ValueInt::GetValue() const {
    return m_iValue;
}


/**
 * @func   SetValue
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
 * @func   operator=
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
 * @func   operator=
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
 * @func   operator==
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator== (
    const ValueInt& rhs
) const {
    return m_iValue == rhs.GetValue();
}

/**
 * @func   operator==
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
 * @func   operator!=
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator!= (
    const ValueInt& rhs
) const {
    return m_iValue != rhs.GetValue();
}

/**
 * @func   operator!=
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
 * @func   operator>
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator>  (
    const ValueInt& rhs
) const {
    return m_iValue > rhs.GetValue();
}

/**
 * @func   operator>
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
 * @func   operator>=
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator>= (
    const ValueInt& rhs
) const {
    return m_iValue >= rhs.GetValue();
}

/**
 * @func   operator>=
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
 * @func   operator<
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueInt::operator< (
    const ValueInt& rhs
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
ValueInt::operator< (
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
    const ValueInt& rhs
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

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator!= (
    const int_t lhs,
    const ValueInt& rhs
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
    const int_t lhs,
    const ValueInt& rhs
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
    const int_t lhs,
    const ValueInt& rhs
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
    const int_t lhs,
    const ValueInt& rhs
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
    const int_t lhs,
    const ValueInt& rhs
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
    const int_t lhs,
    const ValueInt& rhs
) {
    return lhs <= rhs.GetValue();
}
