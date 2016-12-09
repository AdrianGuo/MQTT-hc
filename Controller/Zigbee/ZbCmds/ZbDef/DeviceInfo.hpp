/*
 * DeviceInfo.hpp
 *
 *  Created on: Nov 3, 2016
 *      Author: taho
 */

#ifndef DEVICEINFO_HPP_
#define DEVICEINFO_HPP_

#include <typedefs.h>

#define READ_REQ            (0)
#define WRITE_REQ           (1)


using namespace std;
/******************************************/
/***              General               ***/
/******************************************/
/*
 * Action for controlling
 */
typedef enum {
    //Basic for every device
    DI_Using = 0,
    DI_State,
    DI_Model,
    DI_Manufacturer,
    //PIR, Door, temperature, humidity, illuminance
    DI_Power,
    //Dimmer, Curtain, Fan
    DI_OnOff,
    //IR
    DI_IR_Data,
    //RGB
    DI_RGB_RemainingTime,
    DI_RGB_Red,
    DI_RGB_Green,
    DI_RGB_Blue,
    //Daikin
    DI_Daikin_Local_Temperature,
    DI_Daikin_Cooling_Setpoint,
    DI_Daikin_Heating_Setpoint,
    DI_Daikin_Min_Heat_Limit,
    DI_Daikin_Max_Heat_Limit,
    DI_Daikin_Min_Cool_Limit,
    DI_Daikin_Max_Cool_Limit,
    DI_Daikin_Control_Seq_Operation,
    DI_Daikin_System_Mode,
    DI_Daikin_Fan_Mode,
    DI_Daikin_Fan_Direction
} DeviceInfo;

/*
 * Details of requested content.
 */

typedef struct {
    int_t   ReqFrom = 0;
    bool_t  ReqType; /* READ_REQ - Read Request; WRITE_REQ - Write Req */
    int_t   ReqValue;
} ReqDetails;

/*
 * Details for action
 */
typedef struct DeviceProperty {
    u16_t       DP_ClusterID;
    u16_t       DP_AttributeID;
    u8_t        DP_AttributeDataType;
    u8_t        DP_AttributeDataSize;
    int_t       DP_AttributeData; //data type (4 bytes)!!!
    int_t       DP_PreValue;
    int_t       DP_SetValue;
    bool_t      DP_IsChanged;

    bool_t      DP_IsRequested = FALSE;
    ReqDetails  DP_ReqDetails;

    DeviceInfo  DP_DIName;
    string      DP_DIStringName; //for json message.

    /*
     * Overloaded operators
     */
    inline DeviceProperty&
    operator=(
        const DeviceProperty& rhs
    ){
        DP_ClusterID            = rhs.DP_ClusterID;
        DP_AttributeID          = rhs.DP_AttributeID;
        DP_AttributeDataType    = rhs.DP_AttributeDataType;
        DP_AttributeDataSize    = rhs.DP_AttributeDataSize;
        DP_AttributeData        = rhs.DP_AttributeData;
        DP_PreValue             = rhs.DP_PreValue;
        DP_SetValue             = rhs.DP_SetValue;
        DP_IsChanged            = rhs.DP_IsChanged;
        DP_DIName               = rhs.DP_DIName;
        DP_DIName               = rhs.DP_DIName;
        DP_DIStringName         = rhs.DP_DIStringName;
        return *this;
    }

    inline bool
    operator!=(
        const DeviceProperty& rhs
    ){
        if(DP_ClusterID != rhs.DP_ClusterID) return true;
        if(DP_AttributeID != rhs.DP_AttributeID) return true;
        return false;
    }

    inline bool
    operator==(
        const DeviceProperty& rhs
    ){
        if(DP_ClusterID != rhs.DP_ClusterID) return false;
        if(DP_AttributeID != rhs.DP_AttributeID) return false;
        return true;
    }

} DeviceProperty;


typedef Map<DeviceInfo,DeviceProperty> Action_t;

/******************************************/
/***              Door device           ***/
/******************************************/
//Door response value
typedef enum {
 DV_Opened = 0,
 DV_Closed,
 DV_Chaotic
} DoorValue;

/******************************************/
/***              IR device             ***/
/******************************************/
//IR Command
typedef enum {
 IRCMD_Learn = 0x00,
 IRCMD_Stop,
 IRCMD_Active,
 IRCMD_Delete,
 IRCMD_State
} IrCommand;

//IR response value
typedef enum {
 IR_Idle = 0x00,
 IR_Waiting,
 IR_Timeout,
 IR_Success,
 IR_FullMemory,
 IR_Actived,
 IR_Deleted,
 IR_NotFound
} IRValue;

/******************************************/
/***            Daikin device           ***/
/******************************************/
//Control Sequence Of Operation
typedef enum {
 SO_CoolingOnly = 0,
 SO_CoolingReheat,
 SO_HeatingOnly,
 SO_HeatingReheat,
 SO_CoolingHeating4pipes,
 SO_CoolingHeating4pipesReheat
} SequenceOperation;

//System mode
typedef enum {
 SM_Off = 0,
 SM_Auto,
 SM_Cool = 3,
 SM_Heat,
 SM_EmergencyHeating,
 SM_Precooling,
 SM_FanOnly
} SystemMode;

//Fan Mode
typedef enum {
 FM_Off = 0,
 FM_Low,
 FM_Medium,
 FM_High,
 FM_On,
 FM_Auto,
 FM_Smart
} FanMode;

//Fan Direction
typedef enum {
 FD_DirectionP0 = 0,
 FD_DirectionP1,
 FD_DirectionP2,
 FD_DirectionP3,
 FD_DirectionP4,
 FD_DirectionStop,
 FD_DirectionSwing
} FanDirection;

#endif /* DEVICEINFO_HPP_ */
