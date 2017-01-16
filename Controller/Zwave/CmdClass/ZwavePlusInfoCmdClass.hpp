/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwavePlusInfoCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 14 Nov 2016 14:16:12
 *
 ******************************************************************************/
#ifndef ZWAVEPLUSINFO_CMDCLASS_HPP_
#define ZWAVEPLUSINFO_CMDCLASS_HPP_

#include "typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwDbModel.hpp"
#include "ZwCmdClass.hpp"

class ZwavePlusInfoCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    ZwDbModel_p m_pDbModel;

    ZwavePlusInfoCmdClass(u32_t dwHomeId, u8_t byNodeId);

    ValueDevice_p
    HandleZwPlusInfoReport(u8_p pbCommand, u8_t byLength);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~ZwavePlusInfoCmdClass();

    virtual u8_t GetMaxVersion() const { return ZWAVEPLUS_INFO_VERSION_V2; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_ZWAVEPLUS_INFO_V2; }
    static const String GetZwCmdClassName() { return "ZWAVEPLUS_INFO"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
};

typedef ZwavePlusInfoCmdClass  ZwavePlusInfoCmdClass_t;
typedef ZwavePlusInfoCmdClass* ZwavePlusInfoCmdClass_p;

#endif /* !ZWAVEPLUSINFO_CMDCLASS_HPP_ */
