/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: FirmwareUpdateMetaDataCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 00:04:01
 *
 ******************************************************************************/
#ifndef FIRMWARE_UPDATE_MD_CMDCLASS_HPP_
#define FIRMWARE_UPDATE_MD_CMDCLASS_HPP_

#include "Typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class FirmwareUpdateMDCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;

    FirmwareUpdateMDCmdClass(u32_t dwHomeId, u8_t byNodeId) {}
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~FirmwareUpdateMDCmdClass() {}

    virtual u8_t GetMaxVersion() const { return FIRMWARE_UPDATE_MD_VERSION; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_FIRMWARE_UPDATE_MD_V4; }
    static const String GetZwCmdClassName() { return "FIRMWARE_UPDATE_MD"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
};

typedef FirmwareUpdateMDCmdClass  FirmwareUpdateMDCmdClass_t;
typedef FirmwareUpdateMDCmdClass* FirmwareUpdateMDCmdClass_p;

#endif /* !FIRMWARE_UPDATE_MD_CMDCLASS_HPP_ */
