#ifndef ZWCMD_POWER_HPP_
#define ZWCMD_POWER_HPP_

#include "typedefs.h"
#include "IZwDriver.hpp"
#include "Value.hpp"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"

class ValueZwCmdPower : public Value {
public:
    ValueZwCmdPower() {}
    virtual ~ValueZwCmdPower() {}
};

typedef ValueZwCmdPower  ValueZwCmdPower_t;
typedef ValueZwCmdPower* ValueZwCmdPower_p;

class ZwCmdPower : public IZwDriver {
private:
    ValueZwNode_pp              m_ppValueZwNode;
    ValueZwDriver_p             m_pValueZwDriver;
    ValueZwCmdPower_p           m_pValueZwCmdPower;

    ZwCtrllerFunctor_p          m_pZwCtrllerCallbackFunctor;
    JsonSendZwaveSession_p          m_pJsonZwaveSession;

    HandlerRequest_p            m_pHandlerRequest;
    HandlerResponse_p           m_pHandlerResponse;

    void_t HandleSetSleepModeRequest(ZwPacket_p pZwPacket);
    void_t HandleSetWutTimeoutRequest(ZwPacket_p pZwPacket);
public:
    ZwCmdPower();
    virtual ~ZwCmdPower();

    virtual void_t RegisterHandlers();

    virtual void_t ShareNodeObject(void_p pObject);
    virtual void_t ShareCallbackFunctor(void_p pCtrllerCallbackFunctor);
};

typedef ZwCmdPower  ZwCmdPower_t;
typedef ZwCmdPower* ZwCmdPower_p;

#endif // !ZWCMDPOWER_HPP_
