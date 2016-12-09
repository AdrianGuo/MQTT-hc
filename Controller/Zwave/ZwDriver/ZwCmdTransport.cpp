#include "ZwPacket.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "ValuePtr.hpp"
#include "ZwCmdTransport.hpp"

/**
 * @func   ZwCmdTransport
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdTransport::ZwCmdTransport(
) : IZwDriver(),
    m_ValueLstNode (ValueLstNode::GetInstance()),
    m_ValueZwDriver (ValueRef<ValueZwDriver>::GetInstance()) {

    m_pValueZwCmdTransport  = ValuePtr<ValueZwTransport>::GetInstance();
    m_pJsonZwaveSession     = JsonSendZwaveSession::CreateSession();
    m_pZwDbModel            = ZwDbModel::CreateModel("zwave.db");
    m_pHandlerRequest       = HandlerRequest::GetInstance();
    m_pHandlerResponse      = HandlerResponse::GetInstance();
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
ZwCmdTransport::HandleSendDataRequest(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    u8_t byCallbackId = pZwPacket->GetBuffer()[0];

    if (m_ValueZwDriver.expectedCbakId == byCallbackId) {
        u8_t byFunctionId = pZwPacket->GetFunctionId();
        u8_t byTxStatus = pZwPacket->GetBuffer()[1];

        switch (byTxStatus) { // Do something
           case TRANSMIT_COMPLETE_OK:
           case TRANSMIT_COMPLETE_NO_ACK:
           case TRANSMIT_COMPLETE_FAIL:
           case TRANSMIT_ROUTING_NOT_IDLE:
               break;
           }

        if (m_ValueZwDriver.expectedFuncId == byFunctionId) { // set command
            m_ValueZwDriver.expectedCbakId = 0;
            m_ValueZwDriver.expectedCClaId = 0;
            m_ValueZwDriver.expectedFuncId = 0;
            m_ValueZwDriver.expectedNodeId     = 0;
            m_ValueZwDriver.packetSignal->Set();
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
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    u8_t byRetVal = pZwPacket->GetBuffer()[0];
    u8_t byFunctionId = pZwPacket->GetFunctionId();

    if ((0 == m_ValueZwDriver.expectedCbakId) ||
        (byRetVal == FALSE)) { // no callback
        m_ValueZwDriver.expectedCbakId = 0;
        m_ValueZwDriver.expectedCClaId = 0;
        m_ValueZwDriver.expectedFuncId = 0;
        m_ValueZwDriver.expectedNodeId = 0;
        m_ValueZwDriver.packetSignal->Set();
    }

    if (m_ValueZwDriver.expectedFuncId == byFunctionId) { //send command

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
void_t
ZwCmdTransport::RegisterHandlers() {
    // Response
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_DATA,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdTransport::HandleSendDataResponse));

    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_DATA_BRIDGE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdTransport::HandleSendDataBridgeResponse));

    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_DATA_MULTI,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdTransport::HandleSendDataMultiResponse));

    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_DATA_MULTI_BRIDGE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdTransport::HandleSendDataMultiBridgeResponse));

    // Request
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_DATA,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this,
    &ZwCmdTransport::HandleSendDataRequest));

    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_DATA_BRIDGE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this,
    &ZwCmdTransport::HandleSendDataBridgeRequest));

    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_DATA_MULTI,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this,
    &ZwCmdTransport::HandleSendDataMultiRequest));

    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_DATA_MULTI_BRIDGE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this,
    &ZwCmdTransport::HandleSendDataMultiBridgeRequest));
}
