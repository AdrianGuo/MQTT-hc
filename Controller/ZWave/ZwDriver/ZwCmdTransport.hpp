#ifndef ZWCMD_TRANSPORT_HPP_
#define ZWCMD_TRANSPORT_HPP_

#include "typedefs.h"
#include "IZwDriver.hpp"
#include "Value.hpp"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"

class ValueZwCmdTransport : public Value {
public:
    ValueZwCmdTransport() {}
    virtual ~ValueZwCmdTransport() {}
};

typedef ValueZwCmdTransport  ValueZwCmdTransport_t;
typedef ValueZwCmdTransport* ValueZwCmdTransport_p;

class ZwCmdTransport : public IZwDriver {
private:
    ValueZwNode_pp          m_ppValueZwNode;
    ValueZwDriver_p         m_pValueZwDriver;
    ValueZwCmdTransport_p   m_pValueZwCmdTransport;

    ZwCtrllerFunctor_p      m_pZwCtrllerCallbackFunctor;
    JsonSendZwaveSession_p  m_pJsonZwaveSession;

    HandlerRequest_p        m_pHandlerRequest;
    HandlerResponse_p       m_pHandlerResponse;

    void_t HandleSendDataRequest(ZwPacket_p pZwPacket);
    void_t HandleSendDataResponse(ZwPacket_p pZwPacket);

    void_t HandleSendDataBridgeRequest(ZwPacket_p pZwPacket);
    void_t HandleSendDataBridgeResponse(ZwPacket_p pZwPacket);

    void_t HandleSendDataMultiRequest(ZwPacket_p pZwPacket);
    void_t HandleSendDataMultiResponse(ZwPacket_p pZwPacket);

    void_t HandleSendDataMultiBridgeRequest(ZwPacket_p pZwPacket);
    void_t HandleSendDataMultiBridgeResponse(ZwPacket_p pZwPacket);

public:
    ZwCmdTransport();
    virtual ~ZwCmdTransport();

    virtual void_t RegisterHandlers();

    virtual void_t ShareNodeObject(void_p pObject);
    virtual void_t ShareCallbackFunctor(void_p pCtrllerCallbackFunctor);
};

typedef ZwCmdTransport  ZwCmdTransport_t;
typedef ZwCmdTransport* ZwCmdTransport_p;

#endif // !ZWCMD_TRANSPORT_HPP_
