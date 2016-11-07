#include "ValueString.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueString::ValueString(
    String  strValue,
    u8_t    byValueID
) : Value (ValueType_String) {
    m_strValue = strValue;
    m_byValueID = byValueID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueString::~ValueString() {}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ValueString::GetValueID() {
    return m_byValueID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueString::SetValueID(
    u8_t byValueID
) {
    m_byValueID = byValueID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String&
ValueString::GetValue() {
    return m_strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueString::SetValue(
    String strValue
) {
    m_strValue = strValue;
    SetChange();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueString::IsEqual(
    String strValue
) {
    return ((m_strValue.compare(strValue) == 0) ? TRUE : FALSE);
}
