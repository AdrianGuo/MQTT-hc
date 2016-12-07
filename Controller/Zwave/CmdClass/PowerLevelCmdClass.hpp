/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: PowerLevelCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 00:05:24
 *
 ******************************************************************************/
#ifndef POWERLEVEL_CMDCLASS_HPP_
#define POWERLEVEL_CMDCLASS_HPP_

#include "typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class PowerlevelCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    ZwDbModel_p m_pDbModel;

    PowerlevelCmdClass(u32_t dwHomeId, u8_t byNodeId);

public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~PowerlevelCmdClass();

    virtual u8_t GetMaxVersion() const { return  POWERLEVEL_VERSION; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_POWERLEVEL; }
    static const String GetZwCmdClassName() { return "COMMAND_CLASS_POWERLEVEL"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
};

typedef PowerlevelCmdClass  PowerlevelCmdClass_t;
typedef PowerlevelCmdClass* PowerlevelCmdClass_p;

#endif /* !POWERLEVEL_CMDCLASS_HPP_ */
