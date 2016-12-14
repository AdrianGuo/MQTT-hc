/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: NotificationCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 00:01:45
 *
 ******************************************************************************/
#ifndef NOTIFICATION_CMDCLASS_HPP_
#define NOTIFICATION_CMDCLASS_HPP_

#include "typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class NotificationCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;

    NotificationCmdClass(u32_t dwHomeId, u8_t byNodeId);

public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~NotificationCmdClass();

    virtual u8_t GetMaxVersion() const { return  NOTIFICATION_VERSION_V7; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_NOTIFICATION_V7; }
    static const String GetZwCmdClassName() { return "NOTIFICATION"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
};

typedef NotificationCmdClass  NotificationCmdClass_t;
typedef NotificationCmdClass* NotificationCmdClass_p;

#endif /* !NOTIFICATION_CMDCLASS_HPP_ */
