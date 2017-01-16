#include "ValuePtr.hpp"
#include "ZwCmdPower.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdPower::ZwCmdPower(
) : IZwDriver(),
    m_ValueLstNode (ValueLstNode::GetInstance()),
    m_ValueZwDriver (ValueRef<ValueZwDriver>::GetInstance()) {
    m_pValueZwCmdPower  = ValuePtr<ValueZwCmdPower>::GetInstance();
    m_pJsonZwaveSession = JsonSendZwaveSession::CreateSession();
    m_pZwDbModel        = ZwDbModel::CreateModel("zwave.db");
    m_pHandlerRequest   = HandlerRequest::GetInstance();
    m_pHandlerResponse  = HandlerResponse::GetInstance();
    RegisterHandlers();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdPower::~ZwCmdPower() { }

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdPower::HandleSetSleepModeRequest(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdPower::HandleSetWutTimeoutRequest(
    ZwPacket_p pZwPacket
) {
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdPower::RegisterHandlers() {

}
