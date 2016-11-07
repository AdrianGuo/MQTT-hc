#include "debug.hpp"
#include "String.hpp"
#include "ZwMessage.hpp"
#include "ZwPacket.hpp"
#include "ValueArray.hpp"
#include "JsonCommand.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwCtrllerAPI.hpp"
#include "MultiChannelCmdClass.hpp"
#include "AssociationCmdClass.hpp"
#include "JsonMessagePtr.hpp"

#include "JsonZwLstAdd.hpp"
#include "JsonZwLstDel.hpp"
#include "JsonZwResetRes.hpp"

#include "ValueShare.hpp"
#include "ZwCmdCtrller.hpp"

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::SendAddedNode(
    u8_t byNodeID
) {
    if ((m_pZwCtrllerCallbackFunctor == NULL) || (byNodeID == 0)) { return; }

    if (m_ppValueZwNode[byNodeID - 1] == NULL) { return; }

    u8_t byNodeType = m_ppValueZwNode[byNodeID - 1]->GetValue()->GetDeviceType();

    Vector<JsonZwLstAdd::LstAdd_t> zwLstAdd;
    JsonZwLstAdd::LstAdd_t element;
    element.NodeId = byNodeID;
    element.NodeType = byNodeType;
    element.Order = 0;
    zwLstAdd.push_back(element);

    JsonMessagePtr<JsonZwLstAdd> jsonZwLstAdd =  m_pJsonZwaveSession->GetJsonMapping<JsonZwLstAdd>();
    JsonCommand_p pJsonCommand = jsonZwLstAdd->CreateJsonCommand(zwLstAdd);
    pJsonCommand->SetSrcFlag(JsonCommand::Flag::Zwave);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);

    m_pZwCtrllerCallbackFunctor->operator ()(EvAction::None, pJsonCommand);
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::SendSetAssociation(
    u8_t byGroupID,
    u8_t byNodeID
) {
    DEBUG1("SET ASSOCIATION");

    if (m_ppValueZwNode[byNodeID - 1] == NULL) { return; }

    ZwCmdClass_p pZwCmdClass = m_ppValueZwNode[byNodeID - 1]->GetValue()->GetZwCmdClass(
                                        AssociationCmdClass::GetZwCmdClassID());
    if (pZwCmdClass == NULL) { return; }

    ZwMessage_p pZwMessage = ((AssociationCmdClass_p) pZwCmdClass)->Set(byGroupID, 1);

    if (m_pZwCtrllerCallbackFunctor != NULL) {
        m_pZwCtrllerCallbackFunctor->operator ()(EvAction::Pushback, pZwMessage);
    }
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::SendStopAddNodeToNetwork() {
    DEBUG1("STOP ADD");

    ZwMessage_p pZwMessage = new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_ADD_NODE_TO_NETWORK, TRUE);
    pZwMessage->SetZwCommand(ZwMessage::Command::SendData);
    pZwMessage->ResetPacket(2);
    pZwMessage->Push(ADD_NODE_STOP);
    pZwMessage->Push(pZwMessage->GetNextCallbackID());

    if (m_pZwCtrllerCallbackFunctor != NULL) {
        m_pZwCtrllerCallbackFunctor->operator ()(EvAction::Pushback, pZwMessage);
    }
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::SendGetNodeProtocolInfoRespose(
    u8_t byNodeID
) {
    DEBUG1("NODE PROTOCOL INFO");

    ZwMessage_p pZwMessage = new ZwMessage(byNodeID, REQUEST, FUNC_ID_ZW_GET_NODE_PROTOCOL_INFO, TRUE);
    pZwMessage->ResetPacket(1);
    pZwMessage->Push(byNodeID);

    if (m_pZwCtrllerCallbackFunctor != NULL) {
        m_pZwCtrllerCallbackFunctor->operator ()(Pushback, pZwMessage);
    }
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::SendAssignReturnRoute(
    u8_t byDstNodeId,
    u8_t bySrcNodeId
) {
    DEBUG1("ASSIGN ROUTE");

    ZwMessage_p pZwMessage = new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_ASSIGN_RETURN_ROUTE, TRUE);
    pZwMessage->ResetPacket(3);
    pZwMessage->Push(bySrcNodeId);
    pZwMessage->Push(byDstNodeId);
    pZwMessage->Push(pZwMessage->GetNextCallbackID());

    if (m_pZwCtrllerCallbackFunctor != NULL) {
        m_pZwCtrllerCallbackFunctor->operator ()(EvAction::Pushback, pZwMessage);
    }
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::SendDeleteReturnRoute(
    u8_t byNodeId
) {
    DEBUG1("DELETE ROUTE");

    ZwMessage_p pZwMessage = new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_DELETE_RETURN_ROUTE, TRUE);
    pZwMessage->ResetPacket(2);
    pZwMessage->Push(byNodeId);
    pZwMessage->Push(pZwMessage->GetNextCallbackID());

    if (m_pZwCtrllerCallbackFunctor != NULL) {
        m_pZwCtrllerCallbackFunctor->operator ()(EvAction::Pushback, pZwMessage);
    }
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
void_t
ZwCmdCtrller::AddEndpointForMultiChannelNode(
    u8_t byNodeId
) {
    if (m_ppValueZwNode[byNodeId - 1] == NULL) { return; }

    if (m_ppValueZwNode[byNodeId - 1]->GetValue()->IsSupportMultiChannel()) {
        DEBUG1("MULTICHANNEL ENPOINT GET");
        ZwCmdClass_p pZwCmdClass = m_ppValueZwNode[byNodeId - 1]->GetValue()->GetZwCmdClass(
                MultiChannelCmdClass::GetZwCmdClassID());
        ZwMessage_p pZwMessage = ((MultiChannelCmdClass_p) pZwCmdClass)->SendEndpointGet();
        pZwMessage->SetZwCommand(ZwMessage::Command::GetEnPoints);

        if (m_pZwCtrllerCallbackFunctor != NULL) {
            m_pZwCtrllerCallbackFunctor->operator ()(EvAction::Pushback, pZwMessage);
        }
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
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if ((m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID()) &&
        (m_pValueZwDriver->ByExpectedCallbackID == pZwPacket->GetBuffer()[0])) {
        switch (pZwPacket->GetBuffer()[1]) {
        case ADD_NODE_STATUS_LEARN_READY:
            DEBUG1("learn ready");
            m_pValueZwDriver->EvPacketSignal->Set();
            break;

        case ADD_NODE_STATUS_NODE_FOUND:
            DEBUG1("node found");
            break;

        case ADD_NODE_STATUS_ADDING_SLAVE:
            DEBUG1("adding");
            {
                u8_p pBuffer    = pZwPacket->GetBuffer();
                u8_t byNodeID   = pBuffer[2]; // bSource
                u8_t byLength   = pBuffer[3];
                u8_t byBasic    = pBuffer[4];
                u8_t byGeneric  = pBuffer[5];
                u8_t bySpecific = pBuffer[6];

                if ((byNodeID != 0) && (byNodeID != 0xFF) && (m_ppValueZwNode != NULL)) {
                    u32_t dwHomeID = m_pValueZwCmdCtrller->DwHomeID;
                    m_ppValueZwNode[byNodeID - 1] = new ValueZwNode(new ZwNode(dwHomeID, byNodeID));
                    m_ppValueZwNode[byNodeID - 1]->GetValue()->SetNodeID(byNodeID);
                    m_ppValueZwNode[byNodeID - 1]->GetValue()->SetBasic(byBasic);
                    m_ppValueZwNode[byNodeID - 1]->GetValue()->SetGeneric(byGeneric);
                    m_ppValueZwNode[byNodeID - 1]->GetValue()->SetSpecific(bySpecific);

                    for (u8_t i = 0; i < byLength - 2; i++) {
                        m_ppValueZwNode[byNodeID - 1]->GetValue()->AddZwCmdClass(pBuffer[7 + i]);
                    }
                }
            }
            break;

        case ADD_NODE_STATUS_ADDING_CONTROLLER:
            DEBUG1("adding controller");
            break;

        case ADD_NODE_STATUS_PROTOCOL_DONE:
            DEBUG1("protocol done");
            {
                u8_t byNodeID = pZwPacket->GetBuffer()[2];

                m_pValueZwDriver->ByExpectedCallbackID = 0;
                m_pValueZwDriver->ByExpectedNodeID = 0;
                m_pValueZwDriver->ByExpectedFunctionID = 0;
                m_pValueZwDriver->ByExpectedCmdClassID = 0;

                SendAddedNode(byNodeID); // Add root
                SendStopAddNodeToNetwork();
                SendGetNodeProtocolInfoRespose(byNodeID);
                SendDeleteReturnRoute(byNodeID);
                SendAssignReturnRoute(1, byNodeID);
                SendSetAssociation(1, byNodeID);
                AddEndpointForMultiChannelNode(byNodeID);

                if (m_pZwCtrllerCallbackFunctor != NULL) {
                    m_pZwCtrllerCallbackFunctor->operator ()(EvAction::Set, NULL);
                }
            }
            break;

        case ADD_NODE_STATUS_DONE:
            DEBUG1("add done");

            break;

        case ADD_NODE_STATUS_FAILED:
            DEBUG1("add fail");
            break;

        case ADD_NODE_STATUS_NOT_PRIMARY:
            DEBUG1("not primary");
            break;

        }
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAreNodesNeighboursResponse(
    ZwPacket_p pZwPacket
) { }


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAssignReturnRouteRequest(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if ((m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID()) &&
        (m_pValueZwDriver->ByExpectedCallbackID == pZwPacket->GetBuffer()[0])) {
        u8_t byStatus = pZwPacket->GetBuffer()[1];
        DEBUG2("bStatus: %d", byStatus);

        m_pValueZwDriver->ByExpectedCallbackID = 0;
        m_pValueZwDriver->ByExpectedNodeID = 0;
        m_pValueZwDriver->ByExpectedFunctionID = 0;
        m_pValueZwDriver->ByExpectedCmdClassID = 0;
    }
    m_pValueZwDriver->EvPacketSignal->Set();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleAssignReturnRouteResponse(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID()){
        u8_t byRetVal = pZwPacket->GetBuffer()[0];
        DEBUG2("reVal: %d", byRetVal);
    }
}

/**
 * @func   None
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
 * @func   None
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
 * @func   None
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
 * @func   None
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
 * @func   None
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
 * @func   None
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
 * @func   None
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
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleDeleteReturnRouteRequest(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if ((m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID()) &&
        (m_pValueZwDriver->ByExpectedCallbackID == pZwPacket->GetBuffer()[0])) {
        u8_t byStatus = pZwPacket->GetBuffer()[1];
        DEBUG2("bStatus: %d", byStatus);

        m_pValueZwDriver->ByExpectedCallbackID = 0;
        m_pValueZwDriver->ByExpectedNodeID = 0;
        m_pValueZwDriver->ByExpectedFunctionID = 0;
        m_pValueZwDriver->ByExpectedCmdClassID = 0;
    }
    m_pValueZwDriver->EvPacketSignal->Set();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleDeleteReturnRouteResponse(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID()) {
        u8_t byRetVal = pZwPacket->GetBuffer()[0];
        DEBUG2("retVal: %d", byRetVal);
    }
}

/**
 * @func   None
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
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID()){
        switch (pZwPacket->GetBuffer()[0]) {
        case CONTROLLER_IS_SECONDARY:
        case CONTROLLER_ON_OTHER_NETWORK:
        case CONTROLLER_IS_SUC:
        case CONTROLLER_NODEID_SERVER_PRESENT:
        case CONTROLLER_IS_REAL_PRIMARY:
            break;
        }

        m_pValueZwDriver->ByExpectedCallbackID = 0;
        m_pValueZwDriver->ByExpectedNodeID = 0;
        m_pValueZwDriver->ByExpectedFunctionID = 0;
        m_pValueZwDriver->ByExpectedCmdClassID = 0;
    }
    m_pValueZwDriver->EvPacketSignal->Set();
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
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID()) {
        u8_p pBuffer = pZwPacket->GetBuffer();
        u8_t m_byNodeID = m_pValueZwDriver->ByExpectedNodeID;
        u8_t m_byCapability     = pBuffer[0];
        u8_t m_bySecurity       = pBuffer[1];
        u8_t m_byBasic          = pBuffer[3];
        u8_t m_byGeneric        = pBuffer[4];
        u8_t m_bySpecific       = pBuffer[5];

        m_ppValueZwNode[m_byNodeID - 1]->GetValue()->SetCapability(m_byCapability);
        m_ppValueZwNode[m_byNodeID - 1]->GetValue()->SetBasic(m_byBasic);
        m_ppValueZwNode[m_byNodeID - 1]->GetValue()->SetGeneric(m_byGeneric);
        m_ppValueZwNode[m_byNodeID - 1]->GetValue()->SetSpecific(m_bySpecific);
        m_ppValueZwNode[m_byNodeID - 1]->GetValue()->SetSecurity(m_bySecurity);

        m_pValueZwDriver->ByExpectedCallbackID = 0;
        m_pValueZwDriver->ByExpectedNodeID = 0;
        m_pValueZwDriver->ByExpectedFunctionID = 0;
        m_pValueZwDriver->ByExpectedCmdClassID = 0;
    }
    m_pValueZwDriver->EvPacketSignal->Set();
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
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if ((m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID()) &&
        (m_pValueZwDriver->ByExpectedCallbackID == pZwPacket->GetBuffer()[0])) {

        switch (pZwPacket->GetBuffer()[1]) {
        case REMOVE_NODE_STATUS_LEARN_READY:
            DEBUG1("remove ready");
            m_pValueZwDriver->EvPacketSignal->Set();
            break;

        case REMOVE_NODE_STATUS_NODE_FOUND:
            DEBUG1("node found");
            break;

        case REMOVE_NODE_STATUS_REMOVING_SLAVE:
            DEBUG1("remove slave");
            break;

        case REMOVE_NODE_STATUS_REMOVING_CONTROLLER:
            DEBUG1("remove controller")
            break;

        case REMOVE_NODE_STATUS_DONE:
            DEBUG1("remove done");
            m_pValueZwDriver->ByExpectedCallbackID = 0;
            m_pValueZwDriver->ByExpectedNodeID = 0;
            m_pValueZwDriver->ByExpectedFunctionID = 0;
            m_pValueZwDriver->ByExpectedCmdClassID = 0;
            if (m_pZwCtrllerCallbackFunctor != NULL) {
                u8_t byNodeId = pZwPacket->GetBuffer()[2];
                if (byNodeId != 0) {
                    Vector<u8_t> vNodeId;
                    vNodeId.push_back(byNodeId);
                    JsonMessagePtr<JsonZwLstDel> jsonZwLstDel = m_pJsonZwaveSession->GetJsonMapping<JsonZwLstDel>();
                    JsonCommand_p pJsonCommand = jsonZwLstDel->CreateJsonCommand(vNodeId);
                    pJsonCommand->SetSrcFlag(JsonCommand::Flag::Zwave);
                    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);

                    if (m_ppValueZwNode[byNodeId - 1] != NULL) {
                        delete m_ppValueZwNode[byNodeId - 1];
                        m_ppValueZwNode[byNodeId - 1] = NULL;
                    }

                    DEBUG1("del report");
                    m_pZwCtrllerCallbackFunctor->operator ()(EvAction::Set, pJsonCommand);
                } else {
                    DEBUG1("none report");
                    m_pZwCtrllerCallbackFunctor->operator ()(EvAction::Set, NULL);
                }
            }
            break;

        case REMOVE_NODE_STATUS_FAILED:
            DEBUG1("remove failed");
            break;

        case ADD_NODE_STATUS_NOT_PRIMARY:
            DEBUG1("not pirmary");
            break;
        }
    }
}

/**
 * @func   None
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
 * @func   None
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
 * @func   None
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
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleRequestNodeInfoResponse(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID()) {
        m_pValueZwDriver->ByExpectedCallbackID = 0;
        m_pValueZwDriver->ByExpectedNodeID = 0;
        m_pValueZwDriver->ByExpectedFunctionID = 0;
        m_pValueZwDriver->ByExpectedCmdClassID = 0;
    }
    m_pValueZwDriver->EvPacketSignal->Set();
}

/**
 * @func   None
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
 * @func   None
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
 * @func   None
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
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::HandleSetDefaultRequest(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    u8_t byRet;
    if (m_pValueZwDriver->ByExpectedFunctionID == pZwPacket->GetFunctionID() &&
        m_pValueZwDriver->ByExpectedCallbackID == pZwPacket->GetBuffer()[0]) {
        byRet = 0;
    } else {
        byRet = 3;
    }
    JsonMessagePtr<JsonZwResetRes> jsonZwResetRes = m_pJsonZwaveSession->GetJsonMapping<JsonZwResetRes>();
    JsonCommand_p pJsonCommand = jsonZwResetRes->CreateJsonCommand(byRet);
    pJsonCommand->SetSrcFlag(JsonCommand::Flag::Zwave);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);

    m_pZwCtrllerCallbackFunctor->operator ()(EvAction::None, pJsonCommand);

    m_pValueZwDriver->ByExpectedCallbackID = 0;
    m_pValueZwDriver->ByExpectedNodeID = 0;
    m_pValueZwDriver->ByExpectedFunctionID = 0;
    m_pValueZwDriver->ByExpectedCmdClassID = 0;
    m_pValueZwDriver->EvPacketSignal->Set();
}

/**
 * @func   None
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
 * @func   None
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
 * @func   None
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
 * @func   None
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
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdCtrller::ZwCmdCtrller() {
    m_pValueZwDriver            = ValueShare<ValueZwDriver>::GetInstance();
    m_pValueZwCmdCtrller        = ValueShare<ValueZwCmdCtrller>::GetInstance();
    m_ppValueZwNode             = NULL;

    m_pJsonZwaveSession         = JsonSendZwaveSession::CreateSession();
    m_pZwCtrllerCallbackFunctor = NULL;

    m_pHandlerRequest   = HandlerRequest::GetInstance();
    m_pHandlerResponse  = HandlerResponse::GetInstance();
    RegisterHandlers();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdCtrller::~ZwCmdCtrller() { }


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
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleAreNodesNeighboursResponse));
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

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::ShareNodeObject(
    void_p pObject
) {
    m_ppValueZwNode = (ValueZwNode_pp) pObject;
}

/**
 * @func   SetCallbackFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdCtrller::ShareCallbackFunctor(
    void_p pCallbackFunctor
) {
    m_pZwCtrllerCallbackFunctor = (ZwCtrllerFunctor_p) pCallbackFunctor;
}
