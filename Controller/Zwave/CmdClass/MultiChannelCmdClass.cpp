#include <stddef.h>
#include "LogPlus.hpp"
#include "HelperHc.hpp"
#include "ZwDefs.hpp"
#include "ValueByte.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "ZwNode.hpp"
#include "JsonDevLstAdd.hpp"
#include "MultiChannelCmdClass.hpp"
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
MultiChannelCmdClass::MultiChannelCmdClass(
    u32_t dwHomeId,
    u8_t  byNodeId
) : ZwCmdClass (dwHomeId, byNodeId),
    m_boDynamicNumber (FALSE),
    m_boEpIsIdentical (FALSE),
    m_byNumberOfEndpoints (0),
    m_byNumberOfIndividualEndpoints (0),
    m_byNumberOfAggregatedEndpoints (0),
    m_byTransmitOptions (
        TRANSMIT_OPTION_ACK |
        TRANSMIT_OPTION_AUTO_ROUTE |
        TRANSMIT_OPTION_EXPLORE),
    m_pDbModel (ZwDbModel::CreateModel("zwave.db")
) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
MultiChannelCmdClass::CreateZwCmdClass(
    u32_t dwHomeId,
    u8_t  byNodeId
) {
    return new MultiChannelCmdClass(dwHomeId, byNodeId);
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
* @func   GetNumberOfEndpoints
* @brief  None
* @param  None
* @retval None
*/
u8_t
MultiChannelCmdClass::GetNumberOfEndpoints() const {
    return m_byNumberOfEndpoints;
}

/**
* @func   GetNumberOfIndividualEndpoints
* @brief  None
* @param  None
* @retval None
*/
u8_t
MultiChannelCmdClass::GetNumberOfIndividualEndpoints() const {
    return m_byNumberOfIndividualEndpoints;
}

/**
* @func   GetNumberOfAggregatedEndpoints
* @brief  None
* @param  None
* @retval None
*/
u8_t
MultiChannelCmdClass::GetNumberOfAggregatedEndpoints() const {
    return m_byNumberOfAggregatedEndpoints;
}

/**
 * @func   HandleMessage
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
MultiChannelCmdClass::HandleMessage(
    u8_p pbCommand,
    u8_t byLength
) {
    ValueDevice_p pValueDevice = NULL;
    u8_t byCommand = pbCommand[1];
    switch (byCommand) {
    case  MULTI_CHANNEL_END_POINT_REPORT_V4:
        pValueDevice = HandleMultiChannelEndpointReport(&pbCommand[2], byLength - 2);
        break;

    case MULTI_CHANNEL_CAPABILITY_REPORT_V4:
        pValueDevice = HandleMultiChannelCapabilityReport(&pbCommand[2], byLength - 2);
        break;

    case MULTI_CHANNEL_END_POINT_FIND_REPORT_V4:
        pValueDevice = HandleMultiChannelEnpointFindReport(&pbCommand[2], byLength - 2);
        break;

    case MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT_V4:
        pValueDevice = HandleMultiChannelAggregatedMemberReport(&pbCommand[2], byLength - 2);
        break;

    case MULTI_CHANNEL_CMD_ENCAP_V4:
        pValueDevice = HandleMultiChannelEncap(&pbCommand[2], byLength - 2);
        break;
    }

    return pValueDevice;
}

/**
 * @func   GetEndpoint
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
MultiChannelCmdClass::GetEndpoint() {
    u32_t dwCount   = 0;
    u8_t byZwNodeId = GetNodeId();
    u8_t byLength   = 2;
    u8_p pbyBuffer  = new u8_t[byLength];

    LOG_DEBUG("send enpoint %d get", byZwNodeId);

    pbyBuffer[dwCount++] = GetZwCmdClassId(); // Comand Class
    pbyBuffer[dwCount++] = MULTI_CHANNEL_END_POINT_GET_V4; // Command

    ZwMessage_p pZwMsg = new ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA,
    TRUE, TRUE, FUNC_ID_APPLICATION_COMMAND_HANDLER, GetZwCmdClassId());
    pZwMsg->ResetPacket(byLength + 4);

    pZwMsg->Push(byZwNodeId);
    pZwMsg->Push(byLength); // length
    pZwMsg->Push(pbyBuffer, byLength);
    pZwMsg->Push(m_byTransmitOptions);
    pZwMsg->Push(pZwMsg->GetNextCallbackId());

    if (pbyBuffer != NULL) {
        delete[] pbyBuffer;
        pbyBuffer = NULL;
    }

    return pZwMsg;
}

/**
 * @func   GetCapabilitiy
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
MultiChannelCmdClass::GetCapabilitiy(
    u8_t byEndPoint
) {
    u32_t dwCount    = 0;
    u8_t  byZwNodeId = GetNodeId();
    u8_t  byLength   = 3;
    u8_p  pbyBuffer  = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassId(); // Comand Class
    pbyBuffer[dwCount++] = MULTI_CHANNEL_CAPABILITY_GET_V4; // Command
    pbyBuffer[dwCount++] = byEndPoint;

    ZwMessage_p pZwMessage = new ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA,
    TRUE, TRUE, FUNC_ID_APPLICATION_COMMAND_HANDLER, GetZwCmdClassId());
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeId);
    pZwMessage->Push(byLength); // length
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

    if (pbyBuffer != NULL) {
        delete[] pbyBuffer;
        pbyBuffer = NULL;
    }

    return pZwMessage;
}

/**
 * @func   EndpointFind
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
MultiChannelCmdClass::EndpointFind(
    u8_t byGeneric,
    u8_t bySpecific
) {
    u32_t dwCount   = 0;
    u8_t byZwNodeId = GetNodeId();
    u8_t byLength   = 4;
    u8_p pbyBuffer  = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassId(); // Comand Class
    pbyBuffer[dwCount++] = MULTI_CHANNEL_END_POINT_FIND_V4; // Command
    pbyBuffer[dwCount++] = byGeneric;
    pbyBuffer[dwCount++] = bySpecific;

    ZwMessage_p pZwMessage =
    new ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE, TRUE,
    FUNC_ID_APPLICATION_COMMAND_HANDLER, GetZwCmdClassId());
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeId);
    pZwMessage->Push(byLength); // length
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

    if (pbyBuffer != NULL) {
        delete[] pbyBuffer;
        pbyBuffer = NULL;
    }

    return pZwMessage;
}

/**
 * @func   HandleMultiChannelEndpointReport
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
MultiChannelCmdClass::HandleMultiChannelEndpointReport(
    u8_p pbCommand,
    u8_t byLength
) {
    if ((pbCommand[0] & BV(7)) != 0) { m_boDynamicNumber = TRUE; }

    if ((pbCommand[0] & BV(6)) != 0) { m_boEpIsIdentical = TRUE; }

    m_byNumberOfIndividualEndpoints = pbCommand[1];

    if (byLength == 3) { m_byNumberOfAggregatedEndpoints = pbCommand[2]; }

    m_byNumberOfEndpoints =
    m_byNumberOfIndividualEndpoints + m_byNumberOfAggregatedEndpoints;

    ZwNode_p pZwRootNode = GetNode();
    pZwRootNode->SetNbrOfEndpoints(m_byNumberOfEndpoints);

    for (u32_t i = 0; i < m_byNumberOfIndividualEndpoints; i++) {
        ProcGetCapability(pZwRootNode, i + 1);
    }

    return NULL;
}

/**
 * @func   ProcGetCapability
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
MultiChannelCmdClass::ProcGetCapability(
    ZwNode_p pZwNode,
    u8_t byEndpoint
) {
    pZwNode->AddEndpoint(byEndpoint);
    MultiChannelCmdClass_p pMultiChannelCmdClass =
    (MultiChannelCmdClass_p) pZwNode->GetZwCmdClass(GetZwCmdClassId());
    ZwMessage_p pZwMessage = pMultiChannelCmdClass->GetCapabilitiy(byEndpoint);

    pZwMessage->SetExpectedEndpointId(byEndpoint);
    pZwNode->ProcessFunctor(EvAction::Pushback, pZwMessage);

    LOG_DEBUG("send endpoint %d cability", byEndpoint);
}

/**
 * @func   HandleMultiChannelCapabilityReport
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
MultiChannelCmdClass::HandleMultiChannelCapabilityReport(
    u8_p pbCommand,
    u8_t byLength
) {
    ZwNode_p pZwRootNode = GetNode();
    u8_t byRootId   = pZwRootNode->GetNodeId();
    u8_t byEndpoint = pbCommand[0];
    u8_t byGeneric  = pbCommand[1];
    u8_t bySpecific = pbCommand[2];

    LOG_DEBUG("get endpoint %d cability", byEndpoint);

    if (pZwRootNode == NULL) { return NULL; }

    ZwNode_p pZwEndPoint = pZwRootNode->AddEndpoint(byEndpoint);
    pZwEndPoint->SetGeneric(byGeneric);
    pZwEndPoint->SetSpecific(bySpecific);
    pZwEndPoint->SetHomeId(pZwRootNode->GetHomeId());
    pZwEndPoint->SetNodeId(byRootId);
    pZwEndPoint->SetDeviceType();

    ZwDbDevice parentfind = m_pDbModel->Find<ZwDbDeviceItem>().
    Where("NodeId = ?").Bind(byRootId).Where("Position = ?").Bind(0);

    if (parentfind.get() == NULL) { return NULL; }

    ZwDbDevice devicefind = m_pDbModel->Find<ZwDbDeviceItem>().
    Where("NodeId = ?").Bind(byRootId).Where("Position = ?").Bind(byEndpoint);

    JsonDevLstAdd_p jsonZwlstAdd = new JsonDevLstAdd();
    if (devicefind.get() == NULL) {
        Vector<JsonDevLstAdd::Device_t> lstNode;
        JsonDevLstAdd::Device_t node;
        node.devid = (i32_t) byRootId;
        node.order  = byEndpoint;
        node.type   = pZwEndPoint->GetDeviceType();
        node.mac    = "FFFFFFFF";
        lstNode.push_back(node);

        JsonCommand_p pJsonCommand =
        jsonZwlstAdd->CreateJsonCommand(lstNode);

        pZwRootNode->ProcessFunctor(EvAction::None, pJsonCommand);

        LOG_DEBUG("add endpoint %d ", byEndpoint);
        ZwDbDevice device = m_pDbModel->Add(new ZwDbDeviceItem());
        device.Modify()->NodeId   = byRootId;
        device.Modify()->Position = byEndpoint;
        device.Modify()->Generic  = byGeneric;
        device.Modify()->Specific = bySpecific;
        device.Modify()->DevType  = pZwEndPoint->GetDeviceType();
        device.Modify()->Device   = parentfind;

        for (u8_t i = 0; i < byLength; i++) {
            pZwEndPoint->AddZwCmdClass(pbCommand[3 + i]);
            ZwDbCommandClass cmdclass = m_pDbModel->Add(new ZwDbCmdClassItem());
            cmdclass.Modify()->CmdId  = pbCommand[3 + i];
            cmdclass.Modify()->CmdHex = GetHex(pbCommand[3 + i]);
            cmdclass.Modify()->Device = device;
        }
    }

    m_pDbModel->UpdateChanges();
    return NULL;
}

/**
 * @func   HandleMultiChannelEnpointFindReport
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
MultiChannelCmdClass::HandleMultiChannelEnpointFindReport(
    u8_p pbCommand,
    u8_t byLength
) {
    return NULL;
}

/**
 * @func   HandleMultiChannelAggregatedMemberReport
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
MultiChannelCmdClass::HandleMultiChannelAggregatedMemberReport(
    u8_p pbCommand,
    u8_t byLength
) {
    return NULL;
}

/**
 * @func   HandleMultiChannelEncap
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
MultiChannelCmdClass::HandleMultiChannelEncap(
    u8_p pbCommand,
    u8_t byLength
) {
    u8_t bySourceEndpoint = pbCommand[0];
    ZwNode_p pZwaveRootNode = GetNode();
    ZwNode_p pZwaveEndPNode = pZwaveRootNode->AddEndpoint(bySourceEndpoint);

    u8_p pbCommandClass = &pbCommand[2];
    ValueDevice_p pValueDevice =
    pZwaveEndPNode->ApplicationCommandHandler(pbCommandClass, byLength - 2);

    return pValueDevice;
}
