#include <stddef.h>
#include "ValueArray.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueArray::ValueArray(
    u8_t byLength
) : Value (ValueType_Array) {
    if (byLength != 0) {
        m_byCount   = 0;
        m_byLength  = byLength;
        m_pbyValue  = new u8_t[byLength];
    } else {
        m_byCount   = 0;
        m_byLength  = byLength;
        m_pbyValue  = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueArray::~ValueArray() {
    if (m_pbyValue != NULL) {
        delete[] m_pbyValue;
        m_pbyValue = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ValueArray::GetLength() {
    return m_byLength;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_p
ValueArray::GetValue() {
    return m_pbyValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueArray::PushValue(
    u8_t byValue
) {
    if (m_byCount + 1 > m_byLength) {
        return FALSE;
    }
    m_pbyValue[m_byCount++] = byValue;
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueArray::PushArray(
    u8_p pBuffer,
    u8_t byLength
) {
    if (m_byCount + byLength > m_byLength) {
        return FALSE;
    }

    for (u8_t i = 0; i < byLength; i++) {
        m_pbyValue[m_byCount++] = pBuffer[i];
    }
    return TRUE;
}
