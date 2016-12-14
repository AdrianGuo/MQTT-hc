/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: SensorBinaryCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 23:41:23
 *
 ******************************************************************************/
#ifndef SENSORBINARY_CMDCLASS_HPP_
#define SENSORBINARY_CMDCLASS_HPP_

#include "typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class SensorBinaryCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    SensorBinaryCmdClass(u32_t dwHomeId, u8_t byNodeId);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~SensorBinaryCmdClass();

    virtual u8_t GetMaxVersion() const { return  SENSOR_BINARY_VERSION_V2; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_SENSOR_BINARY_V2; }
    static const String GetZwCmdClassName() { return "SENSOR_BINARY"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue()  { return NULL; }
    virtual ZwMessage_p GetSupportedSensor() { return NULL; }
};

typedef SensorBinaryCmdClass  SensorBinaryCmdClass_t;
typedef SensorBinaryCmdClass* SensorBinaryCmdClass_p;







#endif /* !SENSORBINARY_CMDCLASS_HPP_ */
