/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ManufacturerSpecificCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 07:20:26
 *
 ******************************************************************************/
#ifndef MANUFACTURER_SPECIFIC_CMDCLASS_HPP_
#define MANUFACTURER_SPECIFIC_CMDCLASS_HPP_

#include "Typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwDbModel.hpp"
#include "ZwCmdClass.hpp"

class ManufacturerSpecificCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    ZwDbModel_p m_pDbModel;

    ManufacturerSpecificCmdClass(u32_t dwHomeId, u8_t byNodeId);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~ManufacturerSpecificCmdClass();

    virtual u8_t GetMaxVersion() const { return  MANUFACTURER_SPECIFIC_VERSION_V2; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_MANUFACTURER_SPECIFIC_V2; }
    static const String GetZwCmdClassName() { return "MANUFACTURER_SPECIFIC"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
};

typedef ManufacturerSpecificCmdClass  ManufacturerSpecificCmdClass_t;
typedef ManufacturerSpecificCmdClass* ManufacturerSpecificCmdClass_p;

#endif /* !MANUFACTURER_SPECIFIC_CMDCLASS_HPP_ */
