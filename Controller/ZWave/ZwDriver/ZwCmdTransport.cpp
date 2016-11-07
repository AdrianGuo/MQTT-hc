#include "ZwPacket.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "ValueShare.hpp"
#include "ZwCmdTransport.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdTransport::HandleSendDataRequest(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    u8_t byCallbackID = pZwPacket->GetBuffer()[0];

    if (m_pValueZwDriver->ByExpectedCallbackID == byCallbackID) {
        u8_t byFunctionID = pZwPacket->GetFunctionID();
        u8_t byTxStatus = pZwPacket->GetBuffer()[1];

        switch (byTxStatus) { // Do something
           case TRANSMIT_COMPLETE_OK:
           case TRANSMIT_COMPLETE_NO_ACK:
           case TRANSMIT_COMPLETE_FAIL:
           case TRANSMIT_ROUTING_NOT_IDLE:
               break;
           }

        if (m_pValueZwDriver->ByExpectedFunctionID == byFunctionID) { // set command
            m_pValueZwDriver->ByExpectedCallbackID = 0;
            m_pValueZwDriver->ByExpectedCmdClassID = 0;
            m_pValueZwDriver->ByExpectedFunctionID = 0;
            m_pValueZwDriver->ByExpectedNodeID = 0;
            m_pValueZwDriver->EvPacketSignal->Set();
        } else { // report command

        }
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdTransport::HandleSendDataResponse(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    u8_t byRetVal = pZwPacket->GetBuffer()[0];
    u8_t byFunctionID = pZwPacket->GetFunctionID();

    if ((0 == m_pValueZwDriver->ByExpectedCallbackID) || (byRetVal == FALSE)) { // no callback
        m_pValueZwDriver->ByExpectedCallbackID = 0;
        m_pValueZwDriver->ByExpectedCmdClassID = 0;
        m_pValueZwDriver->ByExpectedFunctionID = 0;
        m_pValueZwDriver->ByExpectedNodeID = 0;
        m_pValueZwDriver->EvPacketSignal->Set();
    }

    if (m_pValueZwDriver->ByExpectedFunctionID == byFunctionID) { //send command

    } else { // report command

    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdTransport::HandleSendDataBridgeRequest(
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
ZwCmdTransport::HandleSendDataBridgeResponse(
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
ZwCmdTransport::HandleSendDataMultiRequest(
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
ZwCmdTransport::HandleSendDataMultiResponse(
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
ZwCmdTransport::HandleSendDataMultiBridgeRequest(
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
ZwCmdTransport::HandleSendDataMultiBridgeResponse(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdTransport::ZwCmdTransport() {
    m_pValueZwDriver        = ValueShare<ValueZwDriver>::GetInstance();;
    m_pValueZwCmdTransport  = ValueShare<ValueZwCmdTransport>::GetInstance();
    m_ppValueZwNode         = NULL;

    m_pZwCtrllerCallbackFunctor = NULL;
    m_pJsonZwaveSession         = JsonSendZwaveSession::CreateSession();

    m_pHandlerRequest  = HandlerRequest::GetInstance();
    m_pHandlerResponse = HandlerResponse::GetInstance();
    RegisterHandlers();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdTransport::~ZwCmdTransport() { }

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdTransport::RegisterHandlers() {
    // Response
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_DATA,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdTransport::HandleSendDataResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_DATA_BRIDGE,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdTransport::HandleSendDataBridgeResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_DATA_MULTI,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdTransport::HandleSendDataMultiResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_DATA_MULTI_BRIDGE,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdTransport::HandleSendDataMultiBridgeResponse));

    // Request
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_DATA,
            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdTransport::HandleSendDataRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_DATA_BRIDGE,
            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdTransport::HandleSendDataBridgeRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_DATA_MULTI,
            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdTransport::HandleSendDataMultiRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_DATA_MULTI_BRIDGE,
            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdTransport::HandleSendDataMultiBridgeRequest));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdTransport::ShareNodeObject(
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
ZwCmdTransport::ShareCallbackFunctor(
    void_p pCallbackFunctor
) {
    m_pZwCtrllerCallbackFunctor = (ZwCtrllerFunctor_p) pCallbackFunctor;
}
