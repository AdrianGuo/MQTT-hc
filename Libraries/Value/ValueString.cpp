#include "ValueString.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueString::ValueString(
    String  strValue,
    u8_t    byValueId
) : Value (type_string) {
    m_strValue = strValue;
    m_byValueId = byValueId;
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
ValueString::GetValueId() {
    return m_byValueId;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueString::SetValueId(
    u8_t byValueId
) {
    m_byValueId = byValueId;
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
 * @func   PushBack
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueString::PushBack(
    char c
) {
    m_strValue.push_back(c);
}

/**
 * @func   SetValue
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
