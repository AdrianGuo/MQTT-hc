#include "HelperHc.hpp"
#include "LogPlus.hpp"
#include "String.hpp"
#include "ZwPacket.hpp"
#include "JsonCommand.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwCtrllerAPI.hpp"
#include "JsonMessagePtr.hpp"

#include "JsonDevLstAdd.hpp"
#include "JsonDevLstDel.hpp"
#include "JsonDevResetRes.hpp"

#include "AssociationCmdClass.hpp"
#include "MultiChannelCmdClass.hpp"
#include "ZwMessage.hpp"
#include "ValuePtr.hpp"
#include "ZwCmdCtrller.hpp"

/**
 * @func   ZwCmdCtrller
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdCtrller::ZwCmdCtrller(
) : IZwDriver(),
    m_ValueLstNode (ValueLstNode::GetInstance()),
    m_ValueZwDriver (ValueRef<ValueZwDriver>::GetInstance()),
    m_ValueZwCmdCtrller (ValueRef<ValueZwCmdCtrller>::GetInstance()) {
    m_pJsonZwaveSession  = JsonSendZwaveSession::CreateSession();
    m_pZwDbModel         = ZwDbModel::CreateModel("zwave.db");
    m_pHandlerRequest    = HandlerRequest::GetInstance();
    m_pHandlerResponse   = HandlerResponse::GetInstance();
    RegisterHandlers();
}

/**
 * @func   ~ZwCmdCtrller
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdCtrller::~ZwCmdCtrller() { }

/**
* @func   ProcSendJsonAddedNode
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::ProcSendJsonAddedNode(
    u8_t byNodeId
) {
    if (byNodeId == 0 || m_ValueLstNode[byNodeId - 1] == NULL) { return; }

    u8_t byNodeType = m_ValueLstNode[byNodeId - 1]->GetDeviceType();

    JsonDevLstAdd::Device_t node;
    Vector<JsonDevLstAdd::Device_t> zwLstNode;

    node.devid = (i32_t) byNodeId;
    node.type  = byNodeType;
    node.netwk = ZWAVE_NETW;
    node.order = ROOT_ORDER;
    node.mac   = "FFFFFFFF";

    zwLstNode.push_back(node);

    JsonMessagePtr<JsonDevLstAdd> jsonZwLstAdd =
    m_pJsonZwaveSession->GetJsonMapping<JsonDevLstAdd>();
    JsonCommand_p pJsonCommand = jsonZwLstAdd->CreateJsonCommand(zwLstNode);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::Coord);

    ProcessFunctor(EvAction::None, pJsonCommand);
}

/**
* @func   ProcSetAssociation
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::ProcSetAssociation(
    u8_t byGroupId,
    u8_t byNodeId
) {
    LOG_DEBUG("set association...");

    if (m_ValueLstNode[byNodeId - 1] == NULL) { return; }

    ZwCmdClass_p pZwCmdClass =
    m_ValueLstNode[byNodeId - 1]->GetZwCmdClass(AssociationCmdClass::GetZwCmdClassId());

    if (pZwCmdClass == NULL) { return; }

    ZwMessage_p pZwMessage =
    ((AssociationCmdClass_p) pZwCmdClass)->Set(byGroupId, 1);

    ProcessFunctor(EvAction::Pushback, pZwMessage);
}

/**
* @func   ReConfigZwCtrller
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::ReConfigZwCtrller() {
    LOG_DEBUG("get memory");
    ProcessFunctor(EvAction::Pushback,
    new ZwMessage(0xFF, REQUEST, FUNC_ID_MEMORY_GET_ID, TRUE));

    LOG_DEBUG("get version");
    ProcessFunctor(EvAction::Pushback,
    new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_GET_VERSION, TRUE));

    LOG_DEBUG("serial capabilities");
    ProcessFunctor(EvAction::Pushback,
    new ZwMessage(0xFF, REQUEST, FUNC_ID_SERIAL_API_GET_CAPABILITIES, TRUE));

    LOG_DEBUG("controller capabilities");
    ProcessFunctor(EvAction::Pushback,
    new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_GET_CONTROLLER_CAPABILITIES, TRUE));
}

/**
* @func   ProcStopAddNodeToNetwork
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::ProcResetDatabase() {
    ZwDbController zwCtrller = m_pZwDbModel->Find<ZwDbCtrllerItem>();
    zwCtrller.Remove();
    m_pZwDbModel->UpdateChanges();
}

/**
* @func   ProcStopAddNodeToNetwork
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::ProcStopAddNodeToNetwork() {
    LOG_DEBUG("stop add...");

    ZwMessage_p pZwMessage =
    new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_ADD_NODE_TO_NETWORK, TRUE);
    pZwMessage->SetZwCommand(ZwMessage::Command::SendData);
    pZwMessage->ResetPacket(2);
    pZwMessage->Push(ADD_NODE_STOP);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

    ProcessFunctor(EvAction::Pushback, pZwMessage);
}

/**
* @func   ProcGetNodeProtocolInfoRespose
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::ProcGetNodeProtocolInfoRespose(
    u8_t byNodeId
) {
    LOG_DEBUG("node protocol info...");

    ZwMessage_p pZwMessage =
    new ZwMessage(byNodeId, REQUEST, FUNC_ID_ZW_GET_NODE_PROTOCOL_INFO, TRUE);
    pZwMessage->ResetPacket(1);
    pZwMessage->Push(byNodeId);

    ProcessFunctor(EvAction::Pushback, pZwMessage);
}

/**
* @func   ProcAssignReturnRoute
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::ProcAssignReturnRoute(
    u8_t byDstNodeId,
    u8_t bySrcNodeId
) {
    LOG_DEBUG("assign route...");

    ZwMessage_p pZwMessage =
    new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_ASSIGN_RETURN_ROUTE, TRUE);
    pZwMessage->ResetPacket(3);
    pZwMessage->Push(bySrcNodeId);
    pZwMessage->Push(byDstNodeId);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

    ProcessFunctor(EvAction::Pushback, pZwMessage);
}

/**
* @func   ProcDeleteReturnRoute
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::ProcDeleteReturnRoute(
    u8_t byNodeId
) {
    LOG_DEBUG("delete route...");

    ZwMessage_p pZwMessage =
    new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_DELETE_RETURN_ROUTE, TRUE);
    pZwMessage->ResetPacket(2);
    pZwMessage->Push(byNodeId);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

    ProcessFunctor(EvAction::Pushback, pZwMessage);
}

/**
* @func   ProcAddEndpointMultiChannel
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::ProcAddEndpointMultiChannel(
    u8_t byNodeId
) {
    if (m_ValueLstNode[byNodeId - 1] == NULL) { return; }

    if (m_ValueLstNode[byNodeId - 1]->IsMultiChannel()) {
        LOG_DEBUG("multichannel ep get...");
        ZwCmdClass_p pZwCmdClass =
        m_ValueLstNode[byNodeId - 1]->GetZwCmdClass(
        MultiChannelCmdClass::GetZwCmdClassId());

        ZwMessage_p pZwMessage =
        ((MultiChannelCmdClass_p) pZwCmdClass)->GetEndpoint();
        pZwMessage->SetZwCommand(ZwMessage::Command::SendData);

        ProcessFunctor(EvAction::Pushback, pZwMessage);
    }
}

/**
* @func   HandleAddNodeToNetworkRequest
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::HandleAddNodeToNetworkRequest(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if ((m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId()) &&
        (m_ValueZwDriver.expectedCbakId == pZwPacket->GetBuffer()[0])) {
        switch (pZwPacket->GetBuffer()[1]) {
        case ADD_NODE_STATUS_LEARN_READY:
            LOG_DEBUG("learn ready...");
            m_ValueZwDriver.packetSignal->Set();
            break;

        case ADD_NODE_STATUS_NODE_FOUND:
            LOG_DEBUG("node found");
            break;

        case ADD_NODE_STATUS_ADDING_SLAVE:
            LOG_DEBUG("adding...");
            {
                u8_p pBuffer    = pZwPacket->GetBuffer();
                u8_t byNodeId   = pBuffer[2]; // bSource
                u8_t byLength   = pBuffer[3];
                u8_t byBasic    = pBuffer[4];
                u8_t byGeneric  = pBuffer[5];
                u8_t bySpecific = pBuffer[6];

                if ((byNodeId == 0) || (byNodeId == 0xFF)) {
                    return;
                }

                u32_t dwHomeId = m_ValueZwCmdCtrller.homeId;
                if (m_ValueLstNode[byNodeId - 1] == NULL) {
                    m_ValueLstNode[byNodeId - 1] = new ZwNode(dwHomeId, byNodeId);
                    m_ValueLstNode[byNodeId - 1]->SetNodeId(byNodeId);
                    m_ValueLstNode[byNodeId - 1]->SetBasic(byBasic);
                    m_ValueLstNode[byNodeId - 1]->SetGeneric(byGeneric);
                    m_ValueLstNode[byNodeId - 1]->SetSpecific(bySpecific);
                    m_ValueLstNode[byNodeId - 1]->SetDeviceType();
                    m_ValueLstNode[byNodeId - 1]->SetCallbackFunctor(m_pCallbackFunctor);
                }

                u8_t byTypeDev = m_ValueLstNode[byNodeId - 1]->GetDeviceType();
                ZwDbDevice devicefind = m_pZwDbModel->Find<ZwDbDeviceItem>().
                Where("NodeId = ?").Bind(byNodeId).Where("Position = ?").Bind(0);

                if (devicefind.get() == NULL) {
                    ZwDbController controller = m_pZwDbModel->Find<ZwDbCtrllerItem>();
                    ZwDbDevice     deviceadd  = m_pZwDbModel->Add(new ZwDbDeviceItem());
                    deviceadd.Modify()->NodeId     = byNodeId;
                    deviceadd.Modify()->Position   = ROOT_ORDER;
                    deviceadd.Modify()->DevType    = byTypeDev;
                    deviceadd.Modify()->Basic      = byBasic;
                    deviceadd.Modify()->Generic    = byGeneric;
                    deviceadd.Modify()->Specific   = bySpecific;
                    deviceadd.Modify()->Controller = controller;

                    for (u8_t i = 0; i < byLength - 3; i++) {
                        m_ValueLstNode[byNodeId - 1]->AddZwCmdClass(pBuffer[7 + i]);
                        ZwDbCommandClass cmdclass = m_pZwDbModel->Add(new ZwDbCmdClassItem());
                        cmdclass.Modify()->CmdId  = pBuffer[7 + i];
                        cmdclass.Modify()->CmdHex = GetHex(pBuffer[7 + i]);
                        cmdclass.Modify()->Device = deviceadd;
                    }
                    m_pZwDbModel->UpdateChanges();
                }
            }
            break;

        case ADD_NODE_STATUS_ADDING_CONTROLLER:
            LOG_DEBUG("adding controller...");
            break;

        case ADD_NODE_STATUS_PROTOCOL_DONE:
            LOG_DEBUG("protocol done");
            {
                m_ValueZwDriver.expectedCbakId = 0;
                m_ValueZwDriver.expectedNodeId = 0;
                m_ValueZwDriver.expectedFuncId = 0;
                m_ValueZwDriver.expectedCmdCId = 0;

                ProcessFunctor(EvAction::SSet, NULL);

                u8_t byNodeId = pZwPacket->GetBuffer()[2];

                ProcStopAddNodeToNetwork();

                ProcGetNodeProtocolInfoRespose(byNodeId);

                ProcDeleteReturnRoute(byNodeId);

                ProcAssignReturnRoute(1, byNodeId);

                ProcSetAssociation(1, byNodeId);

                ProcSendJsonAddedNode(byNodeId); // Add root

                ProcAddEndpointMultiChannel(byNodeId);
            }
            break;

        case ADD_NODE_STATUS_DONE:
            LOG_DEBUG("add done");
            m_ValueZwDriver.packetSignal->Set();
            break;

        case ADD_NODE_STATUS_FAILED:
            LOG_DEBUG("add fail");
            m_ValueZwDriver.packetSignal->Set();
            ProcessFunctor(EvAction::SSet, NULL);
            break;

        case ADD_NODE_STATUS_NOT_PRIMARY:
            LOG_DEBUG("not primary");
            break;

        }
    }
}

/**
 * @func   HandleAreNodesNeighboursResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAreNodesNeighboursResponse(
    ZwPacket_p pZwPacket
) { }


/**
 * @func   HandleAssignReturnRouteRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAssignReturnRouteRequest(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if ((m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId()) &&
        (m_ValueZwDriver.expectedCbakId == pZwPacket->GetBuffer()[0])) {
        u8_t byStatus = pZwPacket->GetBuffer()[1];
        LOG_DEBUG("bStatus: %d", byStatus);
        m_ValueZwDriver.expectedCbakId = 0;
        m_ValueZwDriver.expectedNodeId = 0;
        m_ValueZwDriver.expectedFuncId = 0;
        m_ValueZwDriver.expectedCmdCId = 0;
        m_ValueZwDriver.packetSignal->Set();
    }
}

/**
 * @func   HandleAssignReturnRouteResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAssignReturnRouteResponse(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId()){
        u8_t byRetVal = pZwPacket->GetBuffer()[0];
        LOG_DEBUG(" retVal: %d", byRetVal);
    }
}

/**
 * @func   HandleAssignSucReturnRouteRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAssignSucReturnRouteRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleAssignSucReturnRouteResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAssignSucReturnRouteResponse(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   HandleAssignPriorityReturnRouteRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAssignPriorityReturnRouteRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleAssignPriorityReturnRouteResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAssignPriorityReturnRouteResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleAssignPrioritySucReturnRouteRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAssignPrioritySucReturnRouteRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleAssignPrioritySucReturnRouteResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAssignPrioritySucReturnRouteResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleControllerChangeRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleControllerChangeRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleDeleteReturnRouteRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleDeleteReturnRouteRequest(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if ((m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId()) &&
        (m_ValueZwDriver.expectedCbakId == pZwPacket->GetBuffer()[0])) {
        u8_t byStatus = pZwPacket->GetBuffer()[1];
        LOG_DEBUG("bStatus: %d", byStatus);
        m_ValueZwDriver.expectedCbakId = 0;
        m_ValueZwDriver.expectedNodeId = 0;
        m_ValueZwDriver.expectedFuncId = 0;
        m_ValueZwDriver.expectedCmdCId = 0;
        m_ValueZwDriver.packetSignal->Set();
    }
}

/**
 * @func   HandleDeleteReturnRouteResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleDeleteReturnRouteResponse(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId()) {
        u8_t byRetVal = pZwPacket->GetBuffer()[0];
        LOG_DEBUG(" retVal: %d", byRetVal);
    }
}

/**
 * @func   HandleDeleteSucReturnRouteRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleDeleteSucReturnRouteRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleDeleteSUCReturnRouteResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleDeleteSucReturnRouteResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleGetControllerCapabilitiesResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleGetControllerCapabilitiesResponse(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId()){
        switch (pZwPacket->GetBuffer()[0]) {
        case CONTROLLER_IS_SECONDARY:
        case CONTROLLER_ON_OTHER_NETWORK:
        case CONTROLLER_IS_SUC:
        case CONTROLLER_NODEID_SERVER_PRESENT:
        case CONTROLLER_IS_REAL_PRIMARY:
            break;
        }
        m_ValueZwDriver.expectedCbakId = 0;
        m_ValueZwDriver.expectedNodeId = 0;
        m_ValueZwDriver.expectedFuncId = 0;
        m_ValueZwDriver.expectedCmdCId = 0;
        m_ValueZwDriver.packetSignal->Set();
    }
}

/**
 * @func   HandleGetNeighborCountResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleGetNeighborCountResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleGetPriorityRouteResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleGetPriorityRouteResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleSetPriorityRouteRespose
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleSetPriorityRouteRespose(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleGetNodeProtocolInfoRespose
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleGetNodeProtocolInfoRespose(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId()) {
        u8_p pBuffer      = pZwPacket->GetBuffer();
        u8_t byNodeId     = m_ValueZwDriver.expectedNodeId;
        u8_t byCapability = pBuffer[0];
        u8_t bySecurity   = pBuffer[1];
        u8_t byBasic      = pBuffer[3];
        u8_t byGeneric    = pBuffer[4];
        u8_t bySpecific   = pBuffer[5];

        m_ValueLstNode[byNodeId - 1]->SetCapability(byCapability);
        m_ValueLstNode[byNodeId - 1]->SetBasic(byBasic);
        m_ValueLstNode[byNodeId - 1]->SetGeneric(byGeneric);
        m_ValueLstNode[byNodeId - 1]->SetSpecific(bySpecific);
        m_ValueLstNode[byNodeId - 1]->SetSecurity(bySecurity);
        m_ValueLstNode[byNodeId - 1]->SetDeviceType();

        ZwDbDevice devicefind = m_pZwDbModel->Find<ZwDbDeviceItem>().
        Where("NodeId = ?").Bind(byNodeId);

        if (devicefind.get() != NULL) { // update
            devicefind.Modify()->NodeId     = byNodeId;
            devicefind.Modify()->Position   = ROOT_ORDER;
            devicefind.Modify()->DevType    = m_ValueLstNode[byNodeId - 1]->GetDeviceType();
            devicefind.Modify()->Capability = byCapability;
            devicefind.Modify()->Security   = bySecurity;
            devicefind.Modify()->Basic      = byBasic;
            devicefind.Modify()->Generic    = byGeneric;
            devicefind.Modify()->Specific   = bySpecific;
            m_pZwDbModel->Add(devicefind);
        } else { // add new
            ZwDbController controllerfind = m_pZwDbModel->Find<ZwDbCtrllerItem>();
            ZwDbDevice deviceadd = m_pZwDbModel->Add(new ZwDbDeviceItem());
            deviceadd.Modify()->NodeId      = byNodeId;
            deviceadd.Modify()->Position    = ROOT_ORDER;
            deviceadd.Modify()->DevType     = m_ValueLstNode[byNodeId - 1]->GetDeviceType();
            deviceadd.Modify()->Capability  = byCapability;
            deviceadd.Modify()->Security    = bySecurity;
            deviceadd.Modify()->Basic       = byBasic;
            deviceadd.Modify()->Generic     = byGeneric;
            deviceadd.Modify()->Specific    = bySpecific;
            if (controllerfind.get() != NULL) {
                deviceadd.Modify()->Controller = controllerfind;
            }
        }
        m_pZwDbModel->UpdateChanges();

        m_ValueZwDriver.expectedCbakId = 0;
        m_ValueZwDriver.expectedNodeId = 0;
        m_ValueZwDriver.expectedFuncId = 0;
        m_ValueZwDriver.expectedCmdCId = 0;
        m_ValueZwDriver.packetSignal->Set();
    }
}


/**
 * @func   HandleGetRoutingInfoResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleGetRoutingInfoResponse(
    ZwPacket_p pZwPacket
) { }


/**
 * @func   HandleGetSUCNodeIDResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleGetSucNodeIdResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleIsFailedNodeResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleIsFailedNodeResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleRemoveFailedNodeRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleRemoveFailedNodeRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleRemoveFailedNodeResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleRemoveFailedNodeResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleReplaceFailedNodeRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleReplaceFailedNodeRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleReplaceFailedNodeResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleReplaceFailedNodeResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleRemoveNodeFromNetworkRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleRemoveNodeFromNetworkRequest(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if ((m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId()) &&
        (m_ValueZwDriver.expectedCbakId == pZwPacket->GetBuffer()[0])) {

        switch (pZwPacket->GetBuffer()[1]) {
        case REMOVE_NODE_STATUS_LEARN_READY:
            LOG_DEBUG("remove ready");
            m_ValueZwDriver.packetSignal->Set();
            break;

        case REMOVE_NODE_STATUS_NODE_FOUND:
            LOG_DEBUG("node found");
            break;

        case REMOVE_NODE_STATUS_REMOVING_SLAVE:
            LOG_DEBUG("remove slave");
            break;

        case REMOVE_NODE_STATUS_REMOVING_CONTROLLER:
            LOG_DEBUG("remove controller");
            break;

        case REMOVE_NODE_STATUS_DONE:
            LOG_DEBUG("remove done");
            m_ValueZwDriver.expectedCbakId = 0;
            m_ValueZwDriver.expectedNodeId = 0;
            m_ValueZwDriver.expectedFuncId = 0;
            m_ValueZwDriver.expectedCmdCId = 0;
            {
                u8_t byNodeId = pZwPacket->GetBuffer()[2];

                if (byNodeId != 0) {
                    if (m_ValueLstNode[byNodeId - 1] != NULL) {
                        delete m_ValueLstNode[byNodeId - 1];
                        m_ValueLstNode[byNodeId - 1] = NULL;
                    }

                    // Database
                    ZwDbDevice device = m_pZwDbModel->Find<ZwDbDeviceItem>().
                    Where("NodeId = ?").Bind(byNodeId).Where("Position = ?").Bind(ROOT_ORDER);
                    if (device.get() != NULL) {
                        device.Remove();
                        m_pZwDbModel->UpdateChanges();
                    }

                    // Send JsonCommand
                    Vector<JsonDevLstDel::Device_t> lstDelNode;
                    JsonDevLstDel::Device_t delNode;
                    delNode.devid = (i32_t) byNodeId;
                    delNode.netwk = ZWAVE_NETW;
                    delNode.order = ROOT_ORDER;
                    lstDelNode.push_back(delNode);
                    JsonMessagePtr<JsonDevLstDel> jsonZwLstDel =
                    m_pJsonZwaveSession->GetJsonMapping<JsonDevLstDel>();
                    JsonCommand_p pJsonCommand =
                    jsonZwLstDel->CreateJsonCommand(lstDelNode);
                    pJsonCommand->SetDesFlag(JsonCommand::Flag::Coord);

                    LOG_DEBUG("del report");
                    ProcessFunctor(EvAction::SSet, pJsonCommand);
                } else {
                    LOG_DEBUG("none report");
                    ProcessFunctor(EvAction::SSet, NULL);
                }
            }
            break;

        case REMOVE_NODE_STATUS_FAILED:
            LOG_DEBUG("remove failed");
            m_ValueZwDriver.packetSignal->Set();
            ProcessFunctor(EvAction::SSet, NULL);
            break;

        case ADD_NODE_STATUS_NOT_PRIMARY:
            LOG_DEBUG("not pirmary");
            break;
        }
    }
}

/**
 * @func   HandleRemoveNodeIdFromNetworkRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleRemoveNodeIdFromNetworkRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleReplicationSendRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleReplicationSendRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleReplicationSendResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleReplicationSendResponse(
    ZwPacket_p pZwPacket
) {

}


/**
 * @func   HandleRequestNodeInfoResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleRequestNodeInfoResponse(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId()) {
        m_ValueZwDriver.expectedCbakId = 0;
        m_ValueZwDriver.expectedNodeId = 0;
        m_ValueZwDriver.expectedFuncId = 0;
        m_ValueZwDriver.expectedCmdCId = 0;
        m_ValueZwDriver.packetSignal->Set();
    }
}

/**
 * @func   HandleRequestNodeNeighborUpdateRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleRequestNodeNeighborUpdateRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleSendSucIdRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleSendSucIdRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleSendSucIdResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleSendSucIdResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleSetDefaultRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleSetDefaultRequest(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    u8_t byRetVal = 1;
    if (m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId() &&
        m_ValueZwDriver.expectedCbakId == pZwPacket->GetBuffer()[0]) {
        byRetVal = 0;
        ProcResetDatabase();
        ReConfigZwCtrller();
    }

    JsonMessagePtr<JsonDevResetRes> jsonZwResetRes =
    m_pJsonZwaveSession->GetJsonMapping<JsonDevResetRes>();
    JsonDevResetRes::Device_t node;
    node.ret = byRetVal;
    node.netwk = ZWAVE_NETW;
    JsonCommand_p pJsonCommand = jsonZwResetRes->CreateJsonCommand(node);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::Coord);

    ProcessFunctor(EvAction::None, pJsonCommand);

    m_ValueZwDriver.expectedCbakId = 0;
    m_ValueZwDriver.expectedNodeId = 0;
    m_ValueZwDriver.expectedFuncId = 0;
    m_ValueZwDriver.expectedCmdCId = 0;
    m_ValueZwDriver.packetSignal->Set();
}

/**
 * @func   HandleSetLearnModeRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleSetLearnModeRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleSetRoutingInfoResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleSetRoutingInfoResponse(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   HandleSetRoutingMaxResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleSetRoutingMaxResponse(
    ZwPacket_p pZwPacket
) {

}


/**
 * @func   HandleSetSucNodeIdRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleSetSucNodeIdRequest(
    ZwPacket_p pZwPacket
) {

}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleSetSucNodeIdResponse(
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
ZwCmdCtrller::RegisterHandlers() {
    // Response
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_ARE_NODES_NEIGHBOURS,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwCmdCtrller::HandleAreNodesNeighboursResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_ASSIGN_RETURN_ROUTE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleAssignReturnRouteResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_ASSIGN_SUC_RETURN_ROUTE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleAssignSucReturnRouteRequest));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_ASSIGN_PRIORITY_RETURN_ROUTE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleAssignPriorityReturnRouteRequest));

    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_ASSIGN_PRIORITY_SUC_RETURN_ROUTE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleAssignPrioritySucReturnRouteRequest));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_CONTROLLER_CHANGE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleControllerChangeRequest));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_DELETE_RETURN_ROUTE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleDeleteReturnRouteResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_DELETE_SUC_RETURN_ROUTE,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleDeleteSucReturnRouteRequest));

    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_CONTROLLER_CAPABILITIES,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleGetControllerCapabilitiesResponse));
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_NEIGHBOR_COUNT,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleControllerChangeRequest)); //
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_PRIORITY_ROUTE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleGetPriorityRouteResponse));
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SET_PRIORITY_ROUTE,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleDeleteSucReturnRouteRequest)); //

    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_NODE_PROTOCOL_INFO,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleGetNodeProtocolInfoRespose));
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_GET_ROUTING_TABLE_LINE,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleControllerChangeRequest)); //
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_GET_SUC_NODE_ID,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleGetSucNodeIdResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_IS_FAILED_NODE_ID,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleIsFailedNodeResponse));

    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_REMOVE_FAILED_NODE_ID,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleRemoveFailedNodeRequest));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_REPLACE_FAILED_NODE,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleReplaceFailedNodeResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_REPLICATION_SEND_DATA,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleReplicationSendResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_REQUEST_NODE_INFO,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleRequestNodeInfoResponse));

    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SEND_SUC_ID,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSendSucIdResponse));
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SET_ROUTING_INFO,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleReplaceFailedNodeResponse)); //
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SET_ROUTING_MAX,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetRoutingMaxResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_ZW_SET_SUC_NODE_ID,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetSucNodeIdResponse));

    // Request
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_ADD_NODE_TO_NETWORK,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleAddNodeToNetworkRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_ASSIGN_RETURN_ROUTE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleAssignReturnRouteRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_ASSIGN_SUC_RETURN_ROUTE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleAssignSucReturnRouteRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_ASSIGN_PRIORITY_RETURN_ROUTE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleAssignPriorityReturnRouteRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_ASSIGN_PRIORITY_SUC_RETURN_ROUTE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleAssignPrioritySucReturnRouteRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_CONTROLLER_CHANGE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleControllerChangeRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_DELETE_RETURN_ROUTE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleDeleteReturnRouteRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_DELETE_SUC_RETURN_ROUTE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleDeleteSucReturnRouteRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_GET_CONTROLLER_CAPABILITIES,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleGetControllerCapabilitiesResponse));
//    m_pHandlerRequest->RegisterHandler(FUNC_ID_GET_ROUTING_TABLE_LINE,
//            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleControllerChangeRequest)); //
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_REMOVE_FAILED_NODE_ID,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleRemoveFailedNodeRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_REPLACE_FAILED_NODE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleReplaceFailedNodeRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_REMOVE_NODE_FROM_NETWORK,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleRemoveNodeFromNetworkRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_REMOVE_NODE_ID_FROM_NETWORK,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleRemoveNodeIdFromNetworkRequest));
//    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_REPLICATION_SEND_DATA,
//            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleReplicationSendRequest)); //
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_REQUEST_NODE_NEIGHBOR_UPDATE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleRequestNodeNeighborUpdateRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SEND_SUC_ID,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSendSucIdRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_SET_DEFAULT,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetDefaultRequest));
}
