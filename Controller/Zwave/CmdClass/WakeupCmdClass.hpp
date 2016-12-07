/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: WakeupCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 14 Nov 2016 23:56:22
 *
 ******************************************************************************/
#ifndef WAKEUP_CMDCLASS_HPP_
#define WAKEUP_CMDCLASS_HPP_

#include "typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class WakeupCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    WakeupCmdClass(u32_t dwHomeId, u8_t byNodeId);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~WakeupCmdClass();

    virtual u8_t GetMaxVersion() const { return  WAKE_UP_VERSION_V2; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_WAKE_UP_V2; }
    static const String GetZwCmdClassName() { return "COMMAND_CLASS_WAKE_UP"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
};

typedef WakeupCmdClass  WakeupCmdClass_t;
typedef WakeupCmdClass* WakeupCmdClass_p;


#endif /* !WAKEUP_CMDCLASS_HPP_ */
