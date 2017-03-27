/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: VersionCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 00:00:46
 *
 ******************************************************************************/
#ifndef VERSION_CMDCLASS_HPP_
#define VERSION_CMDCLASS_HPP_

#include "Typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class VersionCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    VersionCmdClass(u32_t dwHomeId, u8_t byNodeId);

public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~VersionCmdClass();

    virtual u8_t GetMaxVersion() const { return  VERSION_VERSION_V2; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_VERSION_V2; }
    static const String GetZwCmdClassName() { return "VERSION"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
    virtual ZwMessage_p GetVersionCmdClass(u8_t byCmdClassId);
};

typedef VersionCmdClass  VersionCmdClass_t;
typedef VersionCmdClass* VersionCmdClass_p;

#endif /* !VERSION_CMDCLASS_HPP_ */
