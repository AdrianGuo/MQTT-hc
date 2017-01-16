/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: AlarmCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 00:00:05
 *
 ******************************************************************************/
#ifndef ALARM_CMDCLASS_HPP_
#define ALARM_CMDCLASS_HPP_

#include "typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class AlarmCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;

    AlarmCmdClass(u32_t dwHomeId, u8_t byNodeId) {}
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~AlarmCmdClass() {}

    virtual u8_t GetMaxVersion() const { return ALARM_VERSION_V2; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_ALARM_V2; }
    static const String GetZwCmdClassName() { return "ALARM"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
};

typedef AlarmCmdClass  AlarmCmdClass_t;
typedef AlarmCmdClass* AlarmCmdClass_p;

#endif /* !ALARM_CMDCLASS_HPP_ */
