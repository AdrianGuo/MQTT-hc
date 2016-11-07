#include "ValueIntDb.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb::ValueIntDb(
    int_t  iValue,
    String strColumnName
) : ValueDb (strColumnName, ValueType_Integer),
    m_iValue (iValue) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb::~ValueIntDb() {}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
ValueIntDb::GetValue() const {
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
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueIntDb&
ValueIntDb::operator= (
    const ValueIntDb& rhs
) {
    m_iValue = rhs.GetValue();
    SetColumnName(rhs.GetColumnName());
    SetForeignKeyName(rhs.GetForeignKeyName());
    SetForeignKeyTable(rhs.GetForeignKeyTable());
    SetFlag(rhs.GetFlag());

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
bool_t
ValueIntDb::operator== (
    const ValueIntDb& rhs
) const {
    return (m_iValue == rhs.GetValue()) &&
           (GetColumnName() == rhs.GetColumnName()) &&
           (GetForeignKeyName() == rhs.GetForeignKeyName()) &&
           (GetForeignKeyTable() == rhs.GetForeignKeyTable()) &&
           (GetFlag() == rhs.GetFlag());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator== (
    int_t iValue
) {
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
    return m_iValue != rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator!= (
    int_t iValue
) {
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
    return m_iValue > rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator>  (
    int_t iValue
) {
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
    return m_iValue >= rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator>= (
    int_t iValue
) {
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
    return m_iValue < rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator<  (
    int_t iValue
) {
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
    return m_iValue <= rhs.GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueIntDb::operator<= (
    int_t iValue
) {
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
