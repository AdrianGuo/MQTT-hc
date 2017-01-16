#include "ZwSerialAPI.hpp"
#include "ZwPacket.hpp"
#include "ValuePtr.hpp"
#include "ZwCmdBasic.hpp"

/**
 * @func   ZwCmdBasic
 * @brief  Contructor
 * @param  None
 * @retval None
 */
ZwCmdBasic::ZwCmdBasic(
) : IZwDriver(),
    m_ValueLstNode (ValueLstNode::GetInstance()),
    m_ValueZwDriver (ValueRef<ValueZwDriver>::GetInstance()),
    m_ValueZwCmdBasic (ValueRef<ValueZwCmdBasic>::GetInstance()) {
    m_pJsonZwaveSession = JsonSendZwaveSession::CreateSession();
    m_pZwDbModel = ZwDbModel::CreateModel("zwave.db");
    m_pHandlerRequest = HandlerRequest::GetInstance();
    m_pHandlerResponse = HandlerResponse::GetInstance();
    RegisterHandlers();
}

/**
 * @func   ~ZwCmdBasic
 * @brief  Destructor
 * @param  None
 * @retval None
 */
ZwCmdBasic::~ZwCmdBasic() { }

/**
 * @func   HandleExploreRequestInclusionResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleExploreRequestInclusionResponse(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   HandleExploreRequestExclusionResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleExploreRequestExclusionResponse(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   HandleGetBackgroundRssiResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleGetBackgroundRssiResponse(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   HandleGetProtocolStatusResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleGetProtocolStatusResponse(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   HandleGetRandomWordResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleGetRandomWordResponse(
    ZwPacket_p pZwPacket
) {
}


/**
 * @func   HandleRandomResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleRandomResponse(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   HandleRfPowerLevelSetResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleRfPowerLevelSetResponse(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   HandleRfPowerLevelGetResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleRfPowerLevelGetResponse(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   HandleRequestNetWorkUpdateRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleRequestNetWorkUpdateRequest(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   HandleRequestNetWorkUpdateResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleRequestNetWorkUpdateResponse(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   HandleSendNodeInformationRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleSendNodeInformationRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleSendNodeInformationResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleSendNodeInformationResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleSendTestFrameRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleSendTestFrameRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleSendTestFrameResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleSendTestFrameResponse(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   HandleSetRfReceiveModeRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleSetRfReceiveModeRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleSetRfReceiveModeResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleSetRfReceiveModeResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleTypeLibraryResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleTypeLibraryResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleVersionResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleVersionResponse(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) {  return ; }

    if (m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId()) {
        u8_p pbyBuffer = pZwPacket->GetBuffer();
        String strVersion (12, '0');

        for (u8_t i = 0; i < 12; i++) {
            m_ValueZwCmdBasic.libraryVersion.PushBack(pbyBuffer[i]);
            strVersion[i] = pbyBuffer[i];
        }

        ZwDbController controllerfind = m_pZwDbModel->Find<ZwDbCtrllerItem>();
        if (controllerfind.get() != NULL) {
            controllerfind.Modify()->LibVersion = strVersion;
            m_pZwDbModel->Add(controllerfind);
            m_pZwDbModel->UpdateChanges();
        }

        m_ValueZwDriver.expectedCbakId = 0;
        m_ValueZwDriver.expectedNodeId = 0;
        m_ValueZwDriver.expectedFuncId = 0;
        m_ValueZwDriver.expectedCmdCId = 0;
        m_ValueZwDriver.packetSignal->Set();
    }
}

/**
 * @func   HandleGetTxTimerResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleGetTxTimerResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleGetNetworkStatsResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleGetNetworkStatsResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleClearNetworkStatsResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::HandleClearNetworkStatsResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   RegisterHandlers
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::RegisterHandlers() {
    // Response
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_EXPLORE_REQUEST_INCLUSION,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleExploreRequestInclusionResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_EXPLORE_REQUEST_EXCLUSION,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleExploreRequestExclusionResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_BACKGROUND_RSSI,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleGetBackgroundRssiResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_PROTOCOL_STATUS,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleGetProtocolStatusResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_RANDOM,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleGetRandomWordResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_RANDOM,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleRandomResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_RF_POWER_LEVEL_SET,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleRfPowerLevelSetResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_RF_POWER_LEVEL_GET,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleRfPowerLevelGetResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_REQUEST_NETWORK_UPDATE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleRequestNetWorkUpdateResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_NODE_INFORMATION,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleSendNodeInformationResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_TEST_FRAME,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleSendTestFrameResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SET_RF_RECEIVE_MODE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleSetRfReceiveModeResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_TYPE_LIBRARY,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleTypeLibraryResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_VERSION,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleVersionResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_GET_TX_TIMERS,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleGetTxTimerResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_NETWORK_STATS,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleGetNetworkStatsResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_CLEAR_NETWORK_STATS,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleClearNetworkStatsResponse));

    // Request
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_REQUEST_NETWORK_UPDATE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleSendTestFrameRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_RF_POWER_LEVEL_REDISCOVERY_SET,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleSendTestFrameRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_NODE_INFORMATION,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleSendTestFrameRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_TEST_FRAME,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this,
    &ZwCmdBasic::HandleSendTestFrameRequest));
}
