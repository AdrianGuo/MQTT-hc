#ifndef ZWCMD_CTRLLER_HPP_
#define ZWCMD_CTRLLER_HPP_

#include "typedefs.h"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"

#include "ZwCmdClassMap.hpp"
#include "JsonZwaveSession.hpp"
#include "ZwDbModel.hpp"
#include "IZwDriver.hpp"

#include "ValueByte.hpp"
#include "ValueDword.hpp"
#include "ValueLstNode.hpp"
#include "ValueZwNode.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class ValueZwCmdCtrller {
public:
    ValueDword& homeId;
    ValueByte&  nodeId;

    ValueZwCmdCtrller(
        ValueDword& dwHomeId,
        ValueByte&  byNodeId
    ) : homeId (dwHomeId),
        nodeId (byNodeId) {
    }
    virtual ~ValueZwCmdCtrller() {}
};

typedef ValueZwCmdCtrller  ValueZwCmdCtrller_t;
typedef ValueZwCmdCtrller* ValueZwCmdCtrller_p;

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class ZwCmdCtrller : public IZwDriver {
private:
    ValueLstNode_t& m_ValueLstNode;
    ValueZwDriver_t& m_ValueZwDriver;
    ValueZwCmdCtrller_t& m_ValueZwCmdCtrller;
    ZwCmdClassMap_t& m_ZwCmdClassMap;

    ZwDbModel_p m_pZwDbModel;
    HandlerRequest_p m_pHandlerRequest;
    HandlerResponse_p  m_pHandlerResponse;
    JsonSendZwaveSession_p m_pJsonZwaveSession;

    void_t ProcResetDatabase();
    void_t ReConfigZwCtrller();
    void_t ProcStopAddNodeToNetwork();
    void_t ProcSendJsonAddedNode(u8_t byNodeId);
    void_t ProcDeleteReturnRoute(u8_t byNodeId);
    void_t ProcGetNodeProtocolInfoRespose(u8_t byNodeId);
    void_t ProcSetAssociation(u8_t byGroupId, u8_t byNodeId);
    void_t ProcAssignReturnRoute(u8_t byDstNodeId, u8_t bySrcNodeId);
    void_t ProcAddEndpointMultiChannel(u8_t byNodeId);

    void_t HandleAddNodeToNetworkRequest(ZwPacket_p pZwPacket);

    void_t HandleAreNodesNeighboursResponse(ZwPacket_p pZwPacket);

    void_t HandleAssignReturnRouteRequest (ZwPacket_p pZwPacket);
    void_t HandleAssignReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleAssignSucReturnRouteRequest (ZwPacket_p pZwPacket);
    void_t HandleAssignSucReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleAssignPriorityReturnRouteRequest (ZwPacket_p pZwPacket);
    void_t HandleAssignPriorityReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleAssignPrioritySucReturnRouteRequest (ZwPacket_p pZwPacket);
    void_t HandleAssignPrioritySucReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleControllerChangeRequest(ZwPacket_p pZwPacket);

    void_t HandleDeleteReturnRouteRequest (ZwPacket_p pZwPacket);
    void_t HandleDeleteReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleDeleteSucReturnRouteRequest (ZwPacket_p pZwPacket);
    void_t HandleDeleteSucReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleGetControllerCapabilitiesResponse(ZwPacket_p pZwPacket);

    void_t HandleGetNeighborCountResponse(ZwPacket_p pZwPacket);

    void_t HandleGetPriorityRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleSetPriorityRouteRespose(ZwPacket_p pZwPacket);

    void_t HandleGetNodeProtocolInfoRespose(ZwPacket_p pZwPacket);

    void_t HandleGetRoutingInfoResponse(ZwPacket_p pZwPacket);

    void_t HandleGetSucNodeIdResponse(ZwPacket_p pZwPacket);

    void_t HandleIsFailedNodeResponse(ZwPacket_p pZwPacket);

    void_t HandleRemoveFailedNodeRequest (ZwPacket_p pZwPacket);
    void_t HandleRemoveFailedNodeResponse(ZwPacket_p pZwPacket);

    void_t HandleReplaceFailedNodeRequest (ZwPacket_p pZwPacket);
    void_t HandleReplaceFailedNodeResponse(ZwPacket_p pZwPacket);

    void_t HandleRemoveNodeFromNetworkRequest(ZwPacket_p pZwPacket);

    void_t HandleRemoveNodeIdFromNetworkRequest(ZwPacket_p pZwPacket);

    void_t HandleReplicationSendRequest (ZwPacket_p pZwPacket);
    void_t HandleReplicationSendResponse(ZwPacket_p pZwPacket);

    void_t HandleRequestNodeInfoResponse(ZwPacket_p pZwPacket);

    void_t HandleRequestNodeNeighborUpdateRequest(ZwPacket_p pZwPacket);

    void_t HandleSendSucIdRequest (ZwPacket_p pZwPacket);
    void_t HandleSendSucIdResponse(ZwPacket_p pZwPacket);

    void_t HandleSetDefaultRequest(ZwPacket_p pZwPacket);

    void_t HandleSetLearnModeRequest(ZwPacket_p pZwPacket);

    void_t HandleSetRoutingInfoResponse(ZwPacket_p pZwPacket);

    void_t HandleSetRoutingMaxResponse(ZwPacket_p pZwPacket);

    void_t HandleSetSucNodeIdRequest (ZwPacket_p pZwPacket);
    void_t HandleSetSucNodeIdResponse(ZwPacket_p pZwPacket);
public:
    ZwCmdCtrller();
    virtual ~ZwCmdCtrller();

    virtual void_t RegisterHandlers();
};

typedef ZwCmdCtrller  ZwCmdCtrller_t;
typedef ZwCmdCtrller* ZwCmdCtrller_p;

#endif // !ZWCMD_CTRLLER_HPP_
