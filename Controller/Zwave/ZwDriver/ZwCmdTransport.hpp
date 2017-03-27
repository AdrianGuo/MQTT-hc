#ifndef ZWCMD_TRANSPORT_HPP_
#define ZWCMD_TRANSPORT_HPP_

#include "Typedefs.h"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"
#include "ZwDbModel.hpp"
#include "IZwDriver.hpp"
#include "ValueLstNode.hpp"

class ValueZwTransport {
public:
    ValueZwTransport() {}
    virtual ~ValueZwTransport() {}
};

typedef ValueZwTransport  ValueZwTransport_t;
typedef ValueZwTransport* ValueZwTransport_p;

class ZwCmdTransport : public IZwDriver {
private:
    ValueLstNode_t&         m_ValueLstNode;
    ValueZwDriver_t&        m_ValueZwDriver;
    ValueZwTransport_p      m_pValueZwCmdTransport;

    JsonSendZwaveSession_p  m_pJsonZwaveSession;

    ZwDbModel_p             m_pZwDbModel;
    HandlerRequest_p        m_pHandlerRequest;
    HandlerResponse_p       m_pHandlerResponse;

    void_t HandleSendDataRequest (ZwPacket_p pZwPacket);
    void_t HandleSendDataResponse(ZwPacket_p pZwPacket);

    void_t HandleSendDataBridgeRequest (ZwPacket_p pZwPacket);
    void_t HandleSendDataBridgeResponse(ZwPacket_p pZwPacket);

    void_t HandleSendDataMultiRequest (ZwPacket_p pZwPacket);
    void_t HandleSendDataMultiResponse(ZwPacket_p pZwPacket);

    void_t HandleSendDataMultiBridgeRequest (ZwPacket_p pZwPacket);
    void_t HandleSendDataMultiBridgeResponse(ZwPacket_p pZwPacket);

public:
    ZwCmdTransport();
    virtual ~ZwCmdTransport();

    virtual void_t RegisterHandlers();
};

typedef ZwCmdTransport  ZwCmdTransport_t;
typedef ZwCmdTransport* ZwCmdTransport_p;

#endif // !ZWCMD_TRANSPORT_HPP_
