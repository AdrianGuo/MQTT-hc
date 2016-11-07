#ifndef ZWCMD_CTRLLER_HPP_
#define ZWCMD_CTRLLER_HPP_

#include "typedefs.h"
#include "IZwDriver.hpp"
#include "ValueDword.hpp"
#include "ValueByte.hpp"
#include "ValueZwNode.hpp"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class ValueZwCmdCtrller {
public:
    ValueDword& DwHomeID;
    ValueByte&  ByNodeID;

    ValueZwCmdCtrller(
        ValueDword& dwHomeID,
        ValueByte&  byNodeID
    ) : DwHomeID (dwHomeID),
        ByNodeID (byNodeID) {
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
    ValueZwNode_pp      m_ppValueZwNode;
    ValueZwDriver_p     m_pValueZwDriver;
    ValueZwCmdCtrller_p m_pValueZwCmdCtrller;

    ZwCtrllerFunctor_p  m_pZwCtrllerCallbackFunctor;
    JsonSendZwaveSession_p  m_pJsonZwaveSession;

    HandlerRequest_p    m_pHandlerRequest;
    HandlerResponse_p   m_pHandlerResponse;

    void_t SendAddedNode(u8_t byNodeID);
    void_t SendSetAssociation(u8_t byGroupID, u8_t byNodeID);
    void_t SendStopAddNodeToNetwork();
    void_t SendGetNodeProtocolInfoRespose(u8_t byNodeID);
    void_t SendAssignReturnRoute(u8_t byDstNodeID, u8_t bySrcNodeID);
    void_t SendDeleteReturnRoute(u8_t byNodeID);
    void_t AddEndpointForMultiChannelNode(u8_t byNodeID);

    void_t HandleAddNodeToNetworkRequest(ZwPacket_p pZwPacket);

    void_t HandleAreNodesNeighboursResponse(ZwPacket_p pZwPacket);

    void_t HandleAssignReturnRouteRequest(ZwPacket_p pZwPacket);
    void_t HandleAssignReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleAssignSucReturnRouteRequest(ZwPacket_p pZwPacket);
    void_t HandleAssignSucReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleAssignPriorityReturnRouteRequest(ZwPacket_p pZwPacket);
    void_t HandleAssignPriorityReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleAssignPrioritySucReturnRouteRequest(ZwPacket_p pZwPacket);
    void_t HandleAssignPrioritySucReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleControllerChangeRequest(ZwPacket_p pZwPacket);

    void_t HandleDeleteReturnRouteRequest(ZwPacket_p pZwPacket);
    void_t HandleDeleteReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleDeleteSucReturnRouteRequest(ZwPacket_p pZwPacket);
    void_t HandleDeleteSucReturnRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleGetControllerCapabilitiesResponse(ZwPacket_p pZwPacket);

    void_t HandleGetNeighborCountResponse(ZwPacket_p pZwPacket);

    void_t HandleGetPriorityRouteResponse(ZwPacket_p pZwPacket);

    void_t HandleSetPriorityRouteRespose(ZwPacket_p pZwPacket);

    void_t HandleGetNodeProtocolInfoRespose(ZwPacket_p pZwPacket);

    void_t HandleGetRoutingInfoResponse(ZwPacket_p pZwPacket);

    void_t HandleGetSucNodeIdResponse(ZwPacket_p pZwPacket);

    void_t HandleIsFailedNodeResponse(ZwPacket_p pZwPacket);

    void_t HandleRemoveFailedNodeRequest(ZwPacket_p pZwPacket);
    void_t HandleRemoveFailedNodeResponse(ZwPacket_p pZwPacket);

    void_t HandleReplaceFailedNodeRequest(ZwPacket_p pZwPacket);
    void_t HandleReplaceFailedNodeResponse(ZwPacket_p pZwPacket);

    void_t HandleRemoveNodeFromNetworkRequest(ZwPacket_p pZwPacket);

    void_t HandleRemoveNodeIdFromNetworkRequest(ZwPacket_p pZwPacket);

    void_t HandleReplicationSendRequest(ZwPacket_p pZwPacket);
    void_t HandleReplicationSendResponse(ZwPacket_p pZwPacket);

    void_t HandleRequestNodeInfoResponse(ZwPacket_p pZwPacket);

    void_t HandleRequestNodeNeighborUpdateRequest(ZwPacket_p pZwPacket);

    void_t HandleSendSucIdRequest(ZwPacket_p pZwPacket);
    void_t HandleSendSucIdResponse(ZwPacket_p pZwPacket);

    void_t HandleSetDefaultRequest(ZwPacket_p pZwPacket);

    void_t HandleSetLearnModeRequest(ZwPacket_p pZwPacket);

    void_t HandleSetRoutingInfoResponse(ZwPacket_p pZwPacket);

    void_t HandleSetRoutingMaxResponse(ZwPacket_p pZwPacket);

    void_t HandleSetSucNodeIdRequest(ZwPacket_p pZwPacket);
    void_t HandleSetSucNodeIdResponse(ZwPacket_p pZwPacket);
public:
    ZwCmdCtrller();
    virtual ~ZwCmdCtrller();

    virtual void_t RegisterHandlers();

    virtual void_t ShareNodeObject(void_p pObject);
    virtual void_t ShareCallbackFunctor(void_p pCtrllerCallbackFunctor);
};

typedef ZwCmdCtrller  ZwCmdCtrller_t;
typedef ZwCmdCtrller* ZwCmdCtrller_p;

#endif // !ZWCMD_CTRLLER_HPP_
