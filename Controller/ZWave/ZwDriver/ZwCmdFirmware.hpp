#ifndef ZWCMD_FIRMWARE_HPP_
#define ZWCMD_FIRMWARE_HPP_

#include "typedefs.h"
#include "IZwDriver.hpp"
#include "Value.hpp"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"

class ValueZwCmdFirmware : public Value {
public:
    ValueZwCmdFirmware() {}
    virtual ~ValueZwCmdFirmware() {}
};

typedef ValueZwCmdFirmware  ValueZwCmdFirmware_t;
typedef ValueZwCmdFirmware* ValueZwCmdFirmware_p;

class ZwCmdFirmware : public IZwDriver {
private:
    ValueZwNode_pp              m_ppValueZwNode;
    ValueZwDriver_p             m_pValueZwDriver;
    ValueZwCmdFirmware_p        m_pValueZwCmdFirmware;

    ZwCtrllerFunctor_p          m_pZwCtrllerCallbackFunctor;
    JsonSendZwaveSession_p      m_pJsonZwaveSession;

    HandlerRequest_p            m_pHandlerRequest;
    HandlerResponse_p           m_pHandlerResponse;

    void_t HandleFirmwareUpdateNvmInit(ZwPacket_p pZwPacket);
    void_t HandleFirmwareUpdateNvmSetnewImage(ZwPacket_p pZwPacket);
    void_t HandleFirmwareUpdateNvmGetNewImage(ZwPacket_p pZwPacket);
    void_t HandleFirmwareUpdateNvmUpdateCrc16(ZwPacket_p pZwPacket);
    void_t HandleFirmwareUpdateNvmIsValidCrc16(ZwPacket_p pZwPacket);
    void_t HandleFirmwareUpdateNvmWrite(ZwPacket_p pZwPacket);
public:
    ZwCmdFirmware();
    virtual ~ZwCmdFirmware();

    virtual void_t RegisterHandlers();

    virtual void_t ShareNodeObject(void_p pObject);
    virtual void_t ShareCallbackFunctor(void_p pCtrllerCallbackFunctor);
};

typedef ZwCmdFirmware  ZwCmdFirmware_t;
typedef ZwCmdFirmware* ZwCmdFirmware_p;

#endif /* !ZWCMD_FIRMWARE_HPP_ */
