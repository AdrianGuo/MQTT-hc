#include "debug.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwPacket.hpp"
#include "ValueShare.hpp"
#include "ZwCmdNvm.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdNvm::HandleMemoryGetIdResponse(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID()) {
        u32_t dwHomeID = (pZwPacket->GetBuffer()[0] << 24) |
                         (pZwPacket->GetBuffer()[1] << 16) |
                         (pZwPacket->GetBuffer()[2] << 8)  |
                         (pZwPacket->GetBuffer()[3]);
        u8_t byNodeID = pZwPacket->GetBuffer()[4];

        m_pValueZwCmdNvm->DwHomeID = dwHomeID;
        m_pValueZwCmdNvm->ByNodeID = byNodeID;

        m_pValueZwDriver->ByExpectedCallbackID = 0;
        m_pValueZwDriver->ByExpectedNodeID = 0;
        m_pValueZwDriver->ByExpectedFunctionID = 0;
        m_pValueZwDriver->ByExpectedCmdClassID = 0;
    }
    m_pValueZwDriver->EvPacketSignal->Set();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdNvm::ZwCmdNvm() {
    m_ppValueZwNode     = NULL;
    m_pValueZwDriver    = ValueShare<ValueZwDriver>::GetInstance();
    m_pValueZwCmdNvm    = ValueShare<ValueZwCmdNvm>::GetInstance();

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
ZwCmdNvm::~ZwCmdNvm() { }

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdNvm::RegisterHandlers() {
    // Response
    m_pHandlerResponse->RegisterHandler(FUNC_ID_MEMORY_GET_ID,
        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdNvm::HandleMemoryGetIdResponse));
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_MEMORY_GET_BYTE,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleReplaceFailedNodeResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_MEMORY_PUT_BYTE,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetRoutingMaxResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_MEMORY_GET_BUFFER,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetSucNodeIdResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_MEMORY_PUT_BUFFER,
//            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdNvm::HandleMemoryGetIdResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_NVM_GET_ID,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleReplaceFailedNodeResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_NVM_EXT_READ_LONG_BYTE,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetRoutingMaxResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_NVM_EXT_WRITE_LONG_BYTE,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetSucNodeIdResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_NVM_EXT_READ_LONG_BUFFER,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetRoutingMaxResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_NVM_EXT_WRITE_LONG_BUFFER,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetSucNodeIdResponse)); //

    // Request
    m_pHandlerRequest->RegisterHandler(FUNC_ID_MEMORY_PUT_BUFFER,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdNvm::HandleMemoryPutBufferRequest));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdNvm::HandleMemoryPutBufferRequest(
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
ZwCmdNvm::ShareNodeObject(
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
ZwCmdNvm::ShareCallbackFunctor(
    void_p pCallbackFunctor
) {
    m_pZwCtrllerCallbackFunctor = (ZwCtrllerFunctor_p) pCallbackFunctor;
}
