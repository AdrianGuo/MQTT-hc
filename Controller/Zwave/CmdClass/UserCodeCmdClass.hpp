/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: UserCodeCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 00:19:23
 *
 ******************************************************************************/
#ifndef USERCODE_CMDCLASS_HPP_
#define USERCODE_CMDCLASS_HPP_

class UserCodeCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    ZwDbModel_p m_pDbModel;

    UserCodeCmdClass(u32_t dwHomeId, u8_t byNodeId) {}

public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~UserCodeCmdClass() {}

    virtual u8_t GetMaxVersion() const { return  USER_CODE_VERSION; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_USER_CODE; }
    static const String GetZwCmdClassName() { return "USER_CODE"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
    virtual ZwMessage_p GetSupportedSensor() { return NULL; }
};

typedef UserCodeCmdClass  UserCodeCmdClass_t;
typedef UserCodeCmdClass* UserCodeCmdClass_p;

#endif /* !USERCODE_CMDCLASS_HPP_ */
