#ifndef ZWCMD_APPFUNC_HPP_
#define ZWCMD_APPFUNC_HPP_

#include "typedefs.h"
#include "IZwDriver.hpp"
#include "Value.hpp"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"

class ValueZwAppFunc : public Value {
public:
    ValueZwAppFunc() {}
    virtual ~ValueZwAppFunc() {}
};

typedef ValueZwAppFunc  ValueZwAppFunc_t;
typedef ValueZwAppFunc* ValueZwAppFunc_p;

class ZwCmdAppFunc : public IZwDriver {
private:
    ValueZwNode_pp              m_ppValueZwNode;
    ValueZwDriver_p             m_pValueZwDriver;
    ValueZwAppFunc_p            m_pValueZwAppFunc;

    ZwCtrllerFunctor_p          m_pZwCtrllerCallbackFunctor;
    JsonSendZwaveSession_p      m_pJsonZwaveSession;

    HandlerRequest_p            m_pHandlerRequest;
    HandlerResponse_p           m_pHandlerResponse;

    void_t ProcResetLocally(u8_t byNodeID);
    void_t ProcDeviceReport(u8_t byNodeID, u8_p pbyReceivedCmd, u8_t byLength);

    void_t HandleApplicationCommandHandlerRequest(ZwPacket_p pZwPacket);
    void_t HandleApplicationNodeInformationRequest(ZwPacket_p pZwPacket);
    void_t HandleApplicationSlaveUpdateRequest(ZwPacket_p pZwPacket);
    void_t HandleApplicationControllerUpdateRequest(ZwPacket_p pZwPacket);
public:
    ZwCmdAppFunc();
    virtual ~ZwCmdAppFunc();

    virtual void_t RegisterHandlers();

    virtual void_t ShareNodeObject(void_p pObject);
    virtual void_t ShareCallbackFunctor(void_p pCtrllerCallbackFunctor);
};

typedef ZwCmdAppFunc  ZwCmdAppFunc_t;
typedef ZwCmdAppFunc* ZwCmdAppFunc_p;

#endif /* !ZWCMD_APPFUNC_HPP_ */
