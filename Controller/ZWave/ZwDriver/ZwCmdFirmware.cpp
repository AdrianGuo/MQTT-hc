#include "ValueShare.hpp"
#include "ZwCmdFirmware.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdFirmware::ZwCmdFirmware() {
    m_pValueZwDriver            = ValueShare<ValueZwDriver>::GetInstance();;
    m_pValueZwCmdFirmware       = ValueShare<ValueZwCmdFirmware>::GetInstance();
    m_ppValueZwNode             = NULL;

    m_pZwCtrllerCallbackFunctor = NULL;
    m_pJsonZwaveSession = JsonSendZwaveSession::CreateSession();

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
ZwCmdFirmware::~ZwCmdFirmware() { }

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdFirmware::RegisterHandlers() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdFirmware::ShareNodeObject(
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
ZwCmdFirmware::ShareCallbackFunctor(
    void_p pCallbackFunctor
) {
    m_pZwCtrllerCallbackFunctor = (ZwCtrllerFunctor_p) pCallbackFunctor;
}
