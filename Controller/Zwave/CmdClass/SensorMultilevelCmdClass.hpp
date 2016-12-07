/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: MultilevelSensorCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 14 Nov 2016 23:57:43
 *
 ******************************************************************************/
#ifndef SENSORMULTILEVEL_CMDCLASS_HPP_
#define SENSORMULTILEVEL_CMDCLASS_HPP_

#include "typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class SensorMultilevelCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    ZwDbModel_p m_pDbModel;

    SensorMultilevelCmdClass(u32_t dwHomeId, u8_t byNodeId);

public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~SensorMultilevelCmdClass();

    virtual u8_t GetMaxVersion() const { return  SENSOR_MULTILEVEL_VERSION_V9; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_SENSOR_MULTILEVEL_V9; }
    static const String GetZwCmdClassName() { return "COMMAND_CLASS_SENSOR_MULTILEVEL"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
    virtual ZwMessage_p GetSupportedSensor() { return NULL; }
};

typedef SensorMultilevelCmdClass  SensorMultilevelCmdClass_t;
typedef SensorMultilevelCmdClass* SensorMultilevelCmdClass_p;

#endif /* !SENSORMULTILEVEL_CMDCLASS_HPP_ */
