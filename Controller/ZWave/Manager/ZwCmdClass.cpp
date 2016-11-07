#include "ZwCmdClass.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass::ZwCmdClass(
    u32_t dwHomeID,
    u8_t byNodeID
) : m_dwHomeID(dwHomeID),
    m_byNodeID(byNodeID),
    m_byVersion (0),
    m_boSecured(FALSE) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass::~ZwCmdClass() {}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Value_p
ZwCmdClass::HandleMessage(
    u8_p pBuffer,
    u8_t byLength
) {
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwCmdClass::GetValue() {
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwCmdClass::GetValue(
    Value& inParam
) {
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwCmdClass::SetValue(
    Value& inParam
) {
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
ZwCmdClass::GetHomeID() const {
    return m_dwHomeID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwCmdClass::GetNodeID() const {
    return m_byNodeID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwCmdClass::GetEndpointID() {
    return 0;
}
