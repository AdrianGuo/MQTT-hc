#include "ZwSerialAPI.hpp"
#include "ZwPacket.hpp"
#include "ValueShare.hpp"
#include "ZwCmdBasic.hpp"

/**
 * @func
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
 * @func
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
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t ZwCmdBasic::HandleGetBackgroundRssiResponse(
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
ZwCmdBasic::HandleGetProtocolStatusResponse(
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
ZwCmdBasic::HandleGetRandomWordResponse(
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
ZwCmdBasic::HandleRandomResponse(
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
ZwCmdBasic::HandleRFPowerLevelSetResponse(
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
ZwCmdBasic::HandleRFPowerLevelGetResponse(
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
ZwCmdBasic::HandleRequestNetWorkUpdateRequest(
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
ZwCmdBasic::HandleRequestNetWorkUpdateResponse(
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
ZwCmdBasic::HandleSendNodeInformationRequest(
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
ZwCmdBasic::HandleSendNodeInformationResponse(
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
ZwCmdBasic::HandleSendTestFrameRequest(
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
ZwCmdBasic::HandleSendTestFrameResponse(
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
ZwCmdBasic::HandleSetRFReceiveModeSend(
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
ZwCmdBasic::HandleSetRFReceiveModeResponse(
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
ZwCmdBasic::HandleTypeLibraryResponse(
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
ZwCmdBasic::HandleVersionResponse(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID()) {
        u8_p pbyBuffer = pZwPacket->GetBuffer();

        for (u8_t i = 0; i < 12; i++) {
            m_pValueZwCmdBasic->StrLibraryVersion.GetValue().push_back(pbyBuffer[i]);
        }

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
void_t
ZwCmdBasic::HandleGetTxTimerResponse(
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
ZwCmdBasic::HandleGetNetworkStatsResponse(
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
ZwCmdBasic::HandleClearNetworkStatsResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   ZwCmdBasic
 * @brief  Contructor
 * @param  None
 * @retval None
 */
ZwCmdBasic::ZwCmdBasic() {
    m_pValueZwDriver    = ValueShare<ValueZwDriver>::GetInstance();
    m_pValueZwCmdBasic  = ValueShare<ValueZwCmdBasic>::GetInstance();
    m_ppValueZwNode     = NULL;

    m_pZwCtrllerCallbackFunctor = NULL;
    m_pJsonZwaveSession = JsonSendZwaveSession::CreateSession();

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
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::RegisterHandlers() {
    // Response
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_EXPLORE_REQUEST_INCLUSION,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleExploreRequestInclusionResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_EXPLORE_REQUEST_EXCLUSION,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleExploreRequestExclusionResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_BACKGROUND_RSSI,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleGetBackgroundRssiResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_PROTOCOL_STATUS,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleGetProtocolStatusResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_RANDOM,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleGetRandomWordResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_RANDOM,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleRandomResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_RF_POWER_LEVEL_SET,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleRFPowerLevelSetResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_RF_POWER_LEVEL_GET,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleRFPowerLevelGetResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_REQUEST_NETWORK_UPDATE,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleRequestNetWorkUpdateResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_NODE_INFORMATION,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleSendNodeInformationResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_TEST_FRAME,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleSendTestFrameResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SET_RF_RECEIVE_MODE,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleSetRFReceiveModeResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_TYPE_LIBRARY,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleTypeLibraryResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_VERSION,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleVersionResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_GET_TX_TIMERS,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleGetTxTimerResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_NETWORK_STATS,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleGetNetworkStatsResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_CLEAR_NETWORK_STATS,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdBasic::HandleClearNetworkStatsResponse));

    // Request
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_REQUEST_NETWORK_UPDATE,
            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdBasic::HandleSendTestFrameRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_RF_POWER_LEVEL_REDISCOVERY_SET,
            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdBasic::HandleSendTestFrameRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_NODE_INFORMATION,
            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdBasic::HandleSendTestFrameRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_TEST_FRAME,
            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdBasic::HandleSendTestFrameRequest));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdBasic::ShareNodeObject(
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
ZwCmdBasic::ShareCallbackFunctor(
    void_p pCallbackFunctor
) {
    m_pZwCtrllerCallbackFunctor = (ZwCtrllerFunctor_p) pCallbackFunctor;
}
