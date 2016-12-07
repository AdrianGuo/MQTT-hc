#ifndef ZWCMD_POWER_HPP_
#define ZWCMD_POWER_HPP_

#include "typedefs.h"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"
#include "ZwDbModel.hpp"
#include "IZwDriver.hpp"
#include "ValueLstNode.hpp"

class ValueZwCmdPower {
public:
    ValueZwCmdPower() {}
    virtual ~ValueZwCmdPower() {}
};

typedef ValueZwCmdPower  ValueZwCmdPower_t;
typedef ValueZwCmdPower* ValueZwCmdPower_p;

class ZwCmdPower : public IZwDriver {
private:
    ValueLstNode_t&             m_ValueLstNode;
    ValueZwDriver_t&            m_ValueZwDriver;
    ValueZwCmdPower_p           m_pValueZwCmdPower;

    JsonSendZwaveSession_p      m_pJsonZwaveSession;

    ZwDbModel_p                 m_pZwDbModel;
    HandlerRequest_p            m_pHandlerRequest;
    HandlerResponse_p           m_pHandlerResponse;

    void_t HandleSetSleepModeRequest (ZwPacket_p pZwPacket);
    void_t HandleSetWutTimeoutRequest(ZwPacket_p pZwPacket);
public:
    ZwCmdPower();
    virtual ~ZwCmdPower();

    virtual void_t RegisterHandlers();
};

typedef ZwCmdPower  ZwCmdPower_t;
typedef ZwCmdPower* ZwCmdPower_p;

#endif // !ZWCMDPOWER_HPP_
