#include "ValueShare.hpp"
#include "ZwCmdPower.hpp"

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
ZwCmdPower::ZwCmdPower() {
    m_pValueZwDriver    = ValueShare<ValueZwDriver>::GetInstance();;
    m_pValueZwCmdPower  = ValueShare<ValueZwCmdPower>::GetInstance();
    m_ppValueZwNode     = NULL;

    m_pZwCtrllerCallbackFunctor = NULL;
    m_pJsonZwaveSession         = JsonSendZwaveSession::CreateSession();

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
ZwCmdPower::RegisterHandlers() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdPower::ShareNodeObject(
    void_p pObject
) {
    m_ppValueZwNode = (ValueZwNode_pp) pObject;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdPower::ShareCallbackFunctor(
    void_p pCallbackFunctor
) {
    m_pZwCtrllerCallbackFunctor = (ZwCtrllerFunctor_p) pCallbackFunctor;
}
