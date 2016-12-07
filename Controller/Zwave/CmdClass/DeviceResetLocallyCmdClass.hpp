/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DeviceResetLocallyCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 00:16:29
 *
 ******************************************************************************/
#ifndef DEVICERESET_LOCALLY_CMDCLASS_HPP_
#define DEVICERESET_LOCALLY_CMDCLASS_HPP_

#include "typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class DeviceResetLocallyCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    ZwDbModel_p m_pDbModel;

    DeviceResetLocallyCmdClass(u32_t dwHomeId, u8_t byNodeId);

public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~DeviceResetLocallyCmdClass();

    virtual u8_t GetMaxVersion() const { return DEVICE_RESET_LOCALLY_VERSION; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_DEVICE_RESET_LOCALLY; }
    static const String GetZwCmdClassName() { return "COMMAND_CLASS_DEVICE_RESET_LOCALLY"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
};

typedef DeviceResetLocallyCmdClass  DeviceResetLocallyCmdClass_t;
typedef DeviceResetLocallyCmdClass* DeviceResetLocallyCmdClass_p;

#endif /* !DEVICERESET_LOCALLY_CMDCLASS_HPP_ */
