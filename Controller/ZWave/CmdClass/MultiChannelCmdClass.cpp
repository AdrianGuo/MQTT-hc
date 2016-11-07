#include <stddef.h>
#include "ZwDefs.hpp"
#include "ValueByte.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "MultiChannelCmdClass.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
MultiChannelCmdClass::MultiChannelCmdClass(
    u32_t dwHomeID,
    u8_t  byNodeID
) : ZwCmdClass(dwHomeID, byNodeID) {
    m_boEpIsIdentical = FALSE;
    m_boDynamicNumber = FALSE;
    m_byNumberOfEndpoints = 0;
    m_byNumberOfIndividualEndpoints = 0;
    m_byNumberOfAggregatedEndpoints = 0;
    m_byTransmitOptions = (TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_AUTO_ROUTE | TRANSMIT_OPTION_EXPLORE);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
MultiChannelCmdClass::CreateZwCmdClass(
    u32_t dwHomeID,
    u8_t  byNodeID
) {
    return new MultiChannelCmdClass(dwHomeID, byNodeID);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
MultiChannelCmdClass::~MultiChannelCmdClass() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
const u8_t
MultiChannelCmdClass::GetZwCmdClassID() {
    return COMMAND_CLASS_MULTI_CHANNEL_V4;
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
u8_t
MultiChannelCmdClass::GetMaxVersion() const {
   return  MULTI_CHANNEL_VERSION_V4;
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
u8_t
MultiChannelCmdClass::GetNumberOfEndpoints() const {
    return m_byNumberOfEndpoints;
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
u8_t
MultiChannelCmdClass::GetNumberOfIndividualEndpoints() const {
    return m_byNumberOfIndividualEndpoints;
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
u8_t
MultiChannelCmdClass::GetNumberOfAggregatedEndpoints() const {
    return m_byNumberOfAggregatedEndpoints;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Value_p
MultiChannelCmdClass::HandleMessage(
    u8_p pInBuffer,
    u8_t byLength
) {
    Value_p pOutValue = NULL;
    u8_t byCommand = pInBuffer[1];
    switch (byCommand) {
    case  MULTI_CHANNEL_END_POINT_REPORT_V4:
        HandleMultiChannelEndpointReport(&pInBuffer[2], byLength - 2);
        break;

    case MULTI_CHANNEL_CAPABILITY_REPORT_V4:
        HandleMultiChannelCapabilityReport(&pInBuffer[2], byLength - 2);
        break;

    case MULTI_CHANNEL_END_POINT_FIND_REPORT_V4:
        HandleMultiChannelEnpointFindReport(&pInBuffer[2], byLength - 2);
        break;

    case MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT_V4:
        HandleMultiChannelAggregatedMemberReport(&pInBuffer[2], byLength - 2);
        break;

    case MULTI_CHANNEL_CMD_ENCAP_V4:
        HandleMultiChannelEncap(&pInBuffer[2], byLength - 2);
        break;
    }

    return pOutValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
MultiChannelCmdClass::SendEndpointGet() {
    u32_t dwCount = 0;
    u8_t byZwNodeID = GetNodeID();
    u8_t byLength = 2;
    u8_p pbyBuffer = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassID(); // Comand Class
    pbyBuffer[dwCount++] = MULTI_CHANNEL_END_POINT_GET_V4; // Command

    ZwMessage_p pZwMsg = new ZwMessage(byZwNodeID, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE, TRUE,
            FUNC_ID_APPLICATION_COMMAND_HANDLER, GetZwCmdClassID());
    pZwMsg->ResetPacket(byLength + 4);

    pZwMsg->Push(byZwNodeID);
    pZwMsg->Push(byLength); // length
    pZwMsg->Push(pbyBuffer, byLength);
    pZwMsg->Push(m_byTransmitOptions);
    pZwMsg->Push(pZwMsg->GetNextCallbackID());

    if (pbyBuffer != NULL) {
        delete[] pbyBuffer;
        pbyBuffer = NULL;
    }

    return pZwMsg;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
MultiChannelCmdClass::SendCapabilitiyGet(
    u8_t byEndPoint
) {
    u32_t dwCount = 0;
    u8_t byZwNodeID = GetNodeID();
    u8_t byLength = 3;
    u8_p pbyBuffer = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassID(); // Comand Class
    pbyBuffer[dwCount++] = MULTI_CHANNEL_CAPABILITY_GET_V4; // Command
    pbyBuffer[dwCount++] = byEndPoint;

    ZwMessage_p pZwMessage = new ZwMessage(byZwNodeID, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE, TRUE,
            FUNC_ID_APPLICATION_COMMAND_HANDLER, GetZwCmdClassID());
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeID);
    pZwMessage->Push(byLength); // length
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackID());

    if (pbyBuffer != NULL) {
        delete[] pbyBuffer;
        pbyBuffer = NULL;
    }

    return pZwMessage;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
MultiChannelCmdClass::SendEndpointFind(
    u8_t byGeneric,
    u8_t bySpecific
) {
    u32_t dwCount = 0;
    u8_t byZwNodeID = GetNodeID();
    u8_t byLength = 4;
    u8_p pbyBuffer = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassID(); // Comand Class
    pbyBuffer[dwCount++] = MULTI_CHANNEL_END_POINT_FIND_V4; // Command
    pbyBuffer[dwCount++] = byGeneric;
    pbyBuffer[dwCount++] = bySpecific;

    ZwMessage_p pZwMessage = new ZwMessage(byZwNodeID, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE, TRUE,
            FUNC_ID_APPLICATION_COMMAND_HANDLER, GetZwCmdClassID());
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeID);
    pZwMessage->Push(byLength); // length
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackID());

    if (pbyBuffer != NULL) {
        delete[] pbyBuffer;
        pbyBuffer = NULL;
    }

    return pZwMessage;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
MultiChannelCmdClass::HandleMultiChannelEndpointReport(
    u8_p pInBuffer,
    u8_t byLength
) {
    if ((pInBuffer[0] & BV(7)) != 0) {
        m_boDynamicNumber = TRUE;
    }

    if ((pInBuffer[0] & BV(6)) != 0) {
        m_boEpIsIdentical = TRUE;
    }

    m_byNumberOfIndividualEndpoints = pInBuffer[1];

    if (byLength == 3) {
        m_byNumberOfAggregatedEndpoints = pInBuffer[2];
    }

    m_byNumberOfEndpoints = m_byNumberOfIndividualEndpoints + m_byNumberOfAggregatedEndpoints;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
MultiChannelCmdClass::HandleMultiChannelCapabilityReport(
    u8_p pInBuffer,
    u8_t byLength
) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
MultiChannelCmdClass::HandleMultiChannelEnpointFindReport(
    u8_p pInBuffer,
    u8_t byLength
) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
MultiChannelCmdClass::HandleMultiChannelAggregatedMemberReport(
    u8_p pInBuffer,
    u8_t byLength
) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
MultiChannelCmdClass::HandleMultiChannelEncap(
    u8_p pInBuffer,
    u8_t byLength
) {

}
