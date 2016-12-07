#ifndef ZWCMD_APPFUNC_HPP_
#define ZWCMD_APPFUNC_HPP_

#include "typedefs.h"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"

#include "JsonZwaveSession.hpp"
#include "ZwDbModel.hpp"
#include "IZwDriver.hpp"

#include "ValueDword.hpp"
#include "ValueLstNode.hpp"

class ValueZwAppFunc {
public:
    ValueDword& homeId;

    ValueZwAppFunc(
        ValueDword& dwHomeId
    ) : homeId (dwHomeId) {}

    virtual ~ValueZwAppFunc() {}
};

typedef ValueZwAppFunc  ValueZwAppFunc_t;
typedef ValueZwAppFunc* ValueZwAppFunc_p;

class ZwCmdAppFunc : public IZwDriver {
private:
    ValueLstNode_t&   m_ValueLstNode;
    ValueZwDriver_t&  m_ValueZwDriver;
    ValueZwAppFunc_t& m_ValueZwAppFunc;

    JsonSendZwaveSession_p  m_pJsonZwaveSession;

    ZwDbModel_p  m_pZwDbModel;

    HandlerRequest_p  m_pHandlerRequest;
    HandlerResponse_p m_pHandlerResponse;

    void_t ProcResetLocally(u8_t byNodeId);
    void_t ProcMultiChannel(u8_p pbCommand, u8_t byLength);
    void_t ProcApplicationCommandHandler(u8_t byNodeId, u8_p pbyReceivedCmd, u8_t byLength);

    void_t HandleApplicationCommandHandlerRequest  (ZwPacket_p pZwPacket);
    void_t HandleApplicationNodeInformationRequest (ZwPacket_p pZwPacket);
    void_t HandleApplicationSlaveUpdateRequest     (ZwPacket_p pZwPacket);
    void_t HandleApplicationControllerUpdateRequest(ZwPacket_p pZwPacket);
public:
    ZwCmdAppFunc();
    virtual ~ZwCmdAppFunc();

    virtual void_t RegisterHandlers();
};

typedef ZwCmdAppFunc  ZwCmdAppFunc_t;
typedef ZwCmdAppFunc* ZwCmdAppFunc_p;

#endif /* !ZWCMD_APPFUNC_HPP_ */
