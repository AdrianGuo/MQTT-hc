#ifndef ZWCMD_FIRMWARE_HPP_
#define ZWCMD_FIRMWARE_HPP_

#include "Typedefs.h"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"
#include "ZwDbModel.hpp"
#include "IZwDriver.hpp"
#include "ValueLstNode.hpp"

class ValueZwCmdFirmware {
public:
    ValueZwCmdFirmware() {}
    virtual ~ValueZwCmdFirmware() {}
};

typedef ValueZwCmdFirmware  ValueZwCmdFirmware_t;
typedef ValueZwCmdFirmware* ValueZwCmdFirmware_p;

class ZwCmdFirmware : public IZwDriver {
private:
    ValueLstNode_t&         m_ValueLstNode;
    ValueZwDriver_t&        m_ValueZwDriver;
    ValueZwCmdFirmware_p    m_pValueZwCmdFirmware;

    JsonSendZwaveSession_p  m_pJsonZwaveSession;

    ZwDbModel_p        m_pZwDbModel;
    HandlerRequest_p   m_pHandlerRequest;
    HandlerResponse_p  m_pHandlerResponse;

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
};

typedef ZwCmdFirmware  ZwCmdFirmware_t;
typedef ZwCmdFirmware* ZwCmdFirmware_p;

#endif /* !ZWCMD_FIRMWARE_HPP_ */
