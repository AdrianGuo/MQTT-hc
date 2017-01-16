#include "ValuePtr.hpp"
#include "ZwCmdFirmware.hpp"

/**
 * @func   ZwCmdFirmware
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdFirmware::ZwCmdFirmware(
) : IZwDriver(),
    m_ValueLstNode (ValueLstNode::GetInstance()),
    m_ValueZwDriver (ValueRef<ValueZwDriver>::GetInstance()) {
    m_pValueZwCmdFirmware = ValuePtr<ValueZwCmdFirmware>::GetInstance();
    m_pJsonZwaveSession   = JsonSendZwaveSession::CreateSession();
    m_pZwDbModel          = ZwDbModel::CreateModel("zwave.db");
    m_pHandlerRequest     = HandlerRequest::GetInstance();
    m_pHandlerResponse    = HandlerResponse::GetInstance();
    RegisterHandlers();
}

/**
 * @func   ~ZwCmdFirmware
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdFirmware::~ZwCmdFirmware() { }

/**
 * @func   RegisterHandlers
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdFirmware::RegisterHandlers() {

}
