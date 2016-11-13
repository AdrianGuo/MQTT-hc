#include <zcl.hpp>
#include <zcl_ha.hpp>
#include <DbAction.hpp>
#include <zcl.hpp>
#include <zcl_ha.hpp>
#include <zcl_general.hpp>
#include <zcl_hvac.hpp>
#include <zcl_lighting.hpp>
#include <zcl_closures.hpp>
#include <zcl_ms.hpp>
#include <zcl_ss.hpp>
#include <zcl_lumi.hpp>
#include <ZbConvertValueFrom.hpp>
#include <ZbHelper.hpp>

#include <ZbDeviceDb.hpp>
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbDeviceDb::ZbDeviceDb() :
    DeviceID        (0,  "DeviceID"),
    Network         (0,  "Network"),
    MAC             ("", "MAC"),
    Model           ("", "Model"),
    Manufacturer    ("", "Manufacturer"),
    Endpoint        (0,  "Endpoint"),
    Type            (0,  "Type"),
    ParentID        (0,  "ParentID"),
    ControllerID    (0,  "ControllerID"),
    State           (Action[DeviceInfo::DI_State].DP_AttributeData)
{
    RealType    =   0;
    byMsgCount  =   0;
    m_pLocker   =   new Locker();

    SyncDeviceAction(DeviceInfo::DI_Model,           ZCL_CLUSTER_ID_GEN_BASIC, ATTRID_BASIC_MODEL_ID);
    SyncDeviceAction(DeviceInfo::DI_Manufacturer,    ZCL_CLUSTER_ID_GEN_BASIC, ATTRID_BASIC_MANUFACTURER_NAME);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbDeviceDb::~ZbDeviceDb() { }

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
ZbDeviceDb::GetTableName() {
    return "ZBDEVICE";
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbDeviceDb::IsInterested() {
    bool_t boRet = FALSE;
    switch (Type.GetValue()) {
        case ZCL_HA_DEVICEID_ON_OFF_LIGHT:
        case ZCL_HA_DEVICEID_DIMMABLE_LIGHT:
        case ZCL_HA_DEVICEID_LIGHT_SENSOR:
        case ZCL_HA_DEVICEID_TEMPERATURE_SENSOR:
        case ZCL_HA_DEVICEID_THERMOSTAT:
        case ZCL_HA_DEVICEID_IAS_ZONE:
        case ZCL_HA_DEVICEID_COLORED_DIMMABLE_LIGHT:
        case ZCL_HA_DEVICEID_HEATING_COOLING_UNIT:
        case ZCL_HA_DEVICEID_DOOR_LOCK:
        case ZCL_HA_DEVICEID_SIMPLE_INPUT:
        case ZCL_LUMI_DEVICEID_IR:
            boRet = TRUE;
            break;

        default:
            boRet = FALSE;
            break;
    }

    return boRet;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDeviceDb::ReceiveInforFromDevice(
    DeviceProperties vResponseDP,
    Vector<u8_p>* pvData
){
    u8_t byLimit = vResponseDP.size();
//    DEBUG2("Received information from device %d - RealType: %d.", DeviceID.GetValue(), RealType);

    switch (RealType) {
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case LUMI_DEVICE_SWITCH:
        case LUMI_DEVICE_INPUT: {
            for(u8_t i = 0; i < byLimit; i++) {
                if (vResponseDP[i].DP_DIName == DeviceInfo::DI_State) {
                    State = *(*pvData)[i];
                    ForwardStateToOutside(this);
                }
                delete (*pvData)[i];
            }
        }

            break;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case LUMI_DEVICE_DIMMER:
        case LUMI_DEVICE_CURTAIN:
        case LUMI_DEVICE_FAN: {
            for(u8_t i = 0; i < byLimit; i++) {
                if (vResponseDP[i].DP_DIName == DeviceInfo::DI_State) {
                    State = *(*pvData)[i];
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_OnOff) {
                    Action[DeviceInfo::DI_OnOff].DP_AttributeData = *(*pvData)[i];
                    byMsgCount++;
                }
                delete (*pvData)[i];
            }
            if(byMsgCount >= 2) {
                byMsgCount = 0;
                if(RealType == LUMI_DEVICE_FAN) ForwardFanStateToOutside(this);
                else ForwardDimmerOrCurtainStateToOutside(this);

            }
        }
            break;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case LUMI_DEVICE_IR: {
            for(u8_t i = 0; i < byLimit; i++) {
                if (vResponseDP[i].DP_DIName == DeviceInfo::DI_IR_Data) {
                    u8_p pbyData = (*pvData)[i];
                    State = *pbyData;
                    ++pbyData;
                    Action[DeviceInfo::DI_State].DP_AttributeID = LittleWord(&pbyData);
//                    DEBUG2("IR:___%02X___%04X___", State, Action[DeviceInfo::DI_State].DP_AttributeID);
                    pbyData = NULL;
                }
                delete (*pvData)[i];
            }
        }
            break;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case LUMI_DEVICE_DOOR:
        case LUMI_DEVICE_PIR:
        case LUMI_DEVICE_TEMPERATURE:
        case LUMI_DEVICE_HUMIDITY:
        case LUMI_DEVICE_ILLUMINANCE: {
            for(u8_t i = 0; i < byLimit; i++) {
                if (vResponseDP[i].DP_DIName == DeviceInfo::DI_State) {
                    if (RealType == LUMI_DEVICE_PIR) State = LittleWord(&(*pvData)[i]);
                    else if(RealType == LUMI_DEVICE_TEMPERATURE) State = *((i16_p) (*pvData)[i]);
                    else if((RealType == LUMI_DEVICE_TEMPERATURE) || (RealType == LUMI_DEVICE_ILLUMINANCE))
                        State = *((u16_p) (*pvData)[i]);
                    else State = *(*pvData)[i];
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Power) {
                    Action[DeviceInfo::DI_Power].DP_AttributeData = vResponseDP[i].DP_AttributeData;
                    byMsgCount++;
                }
                delete (*pvData)[i];
            }
            if(Type.GetValue() != 1) { byMsgCount = 0; ForwardSensorStateToOutside(this); }
            else if(byMsgCount >= 2) { byMsgCount = 0; ForwardSensorStateToOutside(this); }
        }
            break;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case LUMI_DEVICE_RGB: {
            for(u8_t i = 0; i < byLimit; i++) {
                if (vResponseDP[i].DP_DIName == DeviceInfo::DI_State) {
                    State = *(*pvData)[i];
                    ForwardRGBStateToOutside(this);
                } else if(vResponseDP[i].DP_DIName == DeviceInfo::DI_RGB_RemainingTime) {
                    Action[DeviceInfo::DI_RGB_RemainingTime].DP_AttributeData = *((u16_p) (*pvData)[i]);
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_RGB_Red) {
                    Action[DeviceInfo::DI_RGB_Red].DP_AttributeData = *(*pvData)[i];
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_RGB_Green) {
                    Action[DeviceInfo::DI_RGB_Green].DP_AttributeData = *(*pvData)[i];
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_RGB_Blue) {
                    Action[DeviceInfo::DI_RGB_Blue].DP_AttributeData = *(*pvData)[i];
                    byMsgCount++;
                }
                delete (*pvData)[i];
            }
            if(byMsgCount >= 3) { byMsgCount = 0; ForwardRGBStateToOutside(this); }
        }
            break;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case LUMI_DEVICE_DAIKIN: {
            for(u8_t i = 0; i < byLimit; i++) {
                if (vResponseDP[i].DP_DIName == DeviceInfo::DI_State) {
                    State = *(*pvData)[i];
                    Action[DeviceInfo::DI_State].DP_IsChanged = TRUE;
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Daikin_Local_Temperature) {
                    Action[DeviceInfo::DI_Daikin_Local_Temperature].DP_AttributeData = *((i16_p) (*pvData)[i]);
                    Action[DeviceInfo::DI_Daikin_Local_Temperature].DP_IsChanged = TRUE;
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Daikin_Cooling_Setpoint) {
                    Action[DeviceInfo::DI_Daikin_Cooling_Setpoint].DP_AttributeData = *((i16_p) (*pvData)[i]);
                    Action[DeviceInfo::DI_Daikin_Cooling_Setpoint].DP_IsChanged = TRUE;
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Daikin_Heating_Setpoint) {
                    Action[DeviceInfo::DI_Daikin_Heating_Setpoint].DP_AttributeData = *((i16_p) (*pvData)[i]);
                    Action[DeviceInfo::DI_Daikin_Heating_Setpoint].DP_IsChanged = TRUE;
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Daikin_Min_Heat_Limit) {
                    Action[DeviceInfo::DI_Daikin_Min_Heat_Limit].DP_AttributeData = *((i16_p) (*pvData)[i]);
                    Action[DeviceInfo::DI_Daikin_Min_Heat_Limit].DP_IsChanged = TRUE;
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Daikin_Max_Heat_Limit) {
                    Action[DeviceInfo::DI_Daikin_Max_Heat_Limit].DP_AttributeData = *((i16_p) (*pvData)[i]);
                    Action[DeviceInfo::DI_Daikin_Max_Heat_Limit].DP_IsChanged = TRUE;
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Daikin_Min_Cool_Limit){
                    Action[DeviceInfo::DI_Daikin_Min_Cool_Limit].DP_AttributeData = *((i16_p) (*pvData)[i]);
                    Action[DeviceInfo::DI_Daikin_Min_Cool_Limit].DP_IsChanged = TRUE;
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Daikin_Max_Cool_Limit) {
                    Action[DeviceInfo::DI_Daikin_Max_Cool_Limit].DP_AttributeData = *((i16_p) (*pvData)[i]);
                    Action[DeviceInfo::DI_Daikin_Max_Cool_Limit].DP_IsChanged = TRUE;
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Daikin_Control_Seq_Operation) {
                    Action[DeviceInfo::DI_Daikin_Control_Seq_Operation].DP_AttributeData = *(*pvData)[i];
                    Action[DeviceInfo::DI_Daikin_Control_Seq_Operation].DP_IsChanged = TRUE;
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Daikin_System_Mode) {
                    Action[DeviceInfo::DI_Daikin_System_Mode].DP_AttributeData = *(*pvData)[i];
                    Action[DeviceInfo::DI_Daikin_System_Mode].DP_IsChanged = TRUE;
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Daikin_Fan_Mode) {
                    Action[DeviceInfo::DI_Daikin_Fan_Mode].DP_AttributeData = *(*pvData)[i];
                    Action[DeviceInfo::DI_Daikin_Fan_Mode].DP_IsChanged = TRUE;
                    byMsgCount++;
                } else if (vResponseDP[i].DP_DIName == DeviceInfo::DI_Daikin_Fan_Direction) {
                    Action[DeviceInfo::DI_Daikin_Fan_Direction].DP_AttributeData = *(*pvData)[i];
                    Action[DeviceInfo::DI_Daikin_Fan_Direction].DP_IsChanged = TRUE;
                    byMsgCount++;
                }
                delete (*pvData)[i];
            }
            if (byMsgCount >= byLimit) { byMsgCount = 0; ForwardDaikinStateToOutside(this); }
        }
            break;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        default:
            //ForwardForErrorProcessing(this);
            for(u8_t i = 0; i < byLimit; i++) {
                delete (*pvData)[i];
            }
            break;
    }

    delete pvData;
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDeviceDb::EnvAttached() {
    if(Type.GetValue() == 1) {
        SyncDeviceAction(DeviceInfo::DI_Power,       ZCL_CLUSTER_ID_GEN_POWER_CFG,                 ATTRID_POWER_CFG_BATTERY_PERCENTAGE);
    }
    if (Type == ZCL_HA_DEVICEID_TEMPERATURE_SENSOR) {
        SyncDeviceAction(DeviceInfo::DI_State,   ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT,    ATTRID_MS_TEMPERATURE_MEASURED_VALUE);
        RealType = LUMI_DEVICE_TEMPERATURE;
    } else if (Type == ZCL_HA_DEVICEID_THERMOSTAT) {
        SyncDeviceAction(DeviceInfo::DI_State,   ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY,          ATTRID_MS_RELATIVE_HUMIDITY_MEASURED_VALUE);
        RealType = LUMI_DEVICE_HUMIDITY;
    } else if (Type == ZCL_HA_DEVICEID_LIGHT_SENSOR) {
        SyncDeviceAction(DeviceInfo::DI_State,   ZCL_CLUSTER_ID_MS_ILLUMINANCE_MEASUREMENT,    ATTRID_MS_ILLUMINANCE_MEASURED_VALUE);
        RealType = LUMI_DEVICE_ILLUMINANCE;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDeviceDb::GenerateDeviceInfo() {


    size_t lastIndex = Model.GetValue().find_last_not_of("0123456789");
    String prefixModel = Model.GetValue().substr(0, lastIndex + 1);

    if (prefixModel == String("LM-SZ")){
        SyncDeviceAction(DeviceInfo::DI_State,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);

        RealType = LUMI_DEVICE_SWITCH;
    } else if (prefixModel == String("LM-DZ")) {
        SyncDeviceAction(DeviceInfo::DI_State,       ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL,   ATTRID_LEVEL_CURRENT_LEVEL);
        SyncDeviceAction(DeviceInfo::DI_OnOff,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);

        RealType = LUMI_DEVICE_DIMMER;
    } else if (prefixModel == String("LM-FZ")) {
        SyncDeviceAction(DeviceInfo::DI_State,       ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL,   ATTRID_LEVEL_CURRENT_LEVEL);
        SyncDeviceAction(DeviceInfo::DI_OnOff,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);

        RealType = LUMI_DEVICE_FAN;
    } else if (prefixModel == String("LM-BZ")) {
        SyncDeviceAction(DeviceInfo::DI_State,       ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL,   ATTRID_LEVEL_CURRENT_LEVEL);
        SyncDeviceAction(DeviceInfo::DI_OnOff,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);

        RealType = LUMI_DEVICE_CURTAIN;
    } else if (prefixModel == String("LM-IR")) {
        SyncDeviceAction(DeviceInfo::DI_IR_Data,       ZCL_CLUSTER_ID_LUMI_IR,           ATTRID_LUMI_IR);
        SyncDeviceAction(DeviceInfo::DI_State,         ZCL_CLUSTER_ID_LUMI_IR,           ATTRID_SS_IAS_ZONE_STATUS);
        RealType = LUMI_DEVICE_IR;
    } else if (prefixModel == String("LM-IPZ")) {
        SyncDeviceAction(DeviceInfo::DI_State,       ZCL_CLUSTER_ID_GEN_BINARY_INPUT,    ATTRID_BINARY_INPUT_PRESENT_VALUE);

        RealType = LUMI_DEVICE_INPUT;
    }   else if (prefixModel == String("LM-PIR")) {
        if (Type == ZCL_HA_DEVICEID_IAS_ZONE) {
            SyncDeviceAction(DeviceInfo::DI_State,   ZCL_CLUSTER_ID_SS_IAS_ZONE,         ATTRID_SS_IAS_ZONE_STATUS);
            SyncDeviceAction(DeviceInfo::DI_Power,   ZCL_CLUSTER_ID_GEN_POWER_CFG,       ATTRID_POWER_CFG_BATTERY_PERCENTAGE);

            RealType = LUMI_DEVICE_PIR;
        } else {
            EnvAttached();
        }
    } else if (prefixModel == String("LM-DOOR")) {
        if (Type == ZCL_HA_DEVICEID_DOOR_LOCK) {
            SyncDeviceAction(DeviceInfo::DI_State,   ZCL_CLUSTER_ID_CLOSURES_DOOR_CONFIG, ATTRID_CLOSURES_DOOR_STATE);
            SyncDeviceAction(DeviceInfo::DI_Power,   ZCL_CLUSTER_ID_GEN_POWER_CFG,        ATTRID_POWER_CFG_BATTERY_PERCENTAGE);
            RealType = LUMI_DEVICE_DOOR;
        } else {
            EnvAttached();
        }
    } else if (prefixModel == String("LM-ENVR")) {
        EnvAttached();
    } else if (prefixModel == String("LM-RGB")) {
        SyncDeviceAction(DeviceInfo::DI_State,                           ZCL_CLUSTER_ID_GEN_ON_OFF,              ATTRID_ON_OFF);
        SyncDeviceAction(DeviceInfo::DI_RGB_RemainingTime,               ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_REMAINING_TIME);
        SyncDeviceAction(DeviceInfo::DI_RGB_Red,                         ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_R_INTENSITY);
        SyncDeviceAction(DeviceInfo::DI_RGB_Green,                       ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_G_INTENSITY);
        SyncDeviceAction(DeviceInfo::DI_RGB_Blue,                        ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_B_INTENSITY);
        RealType = LUMI_DEVICE_RGB;
    } else if(prefixModel == String("LM-INPUT")) {
        SyncDeviceAction(DeviceInfo::DI_State,                           ZCL_CLUSTER_ID_GEN_BINARY_INPUT,        ATTRID_BINARY_INPUT_PRESENT_VALUE);
        RealType = LUMI_DEVICE_INPUT;
    } else if (prefixModel == String("LM-DKZ")) {
        SyncDeviceAction(DeviceInfo::DI_State,                           ZCL_CLUSTER_ID_GEN_ON_OFF,              ATTRID_ON_OFF);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Local_Temperature,        ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_LOCAL_TEMPERATURE);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Cooling_Setpoint,         ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_OCCUPIED_COOLING_SETPOINT);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Heating_Setpoint,         ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_OCCUPIED_HEATING_SETPOINT);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Min_Heat_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MIN_HEAT_SETPOINT_LIMIT);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Max_Heat_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MAX_HEAT_SETPOINT_LIMIT);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Min_Cool_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MIN_COOL_SETPOINT_LIMIT);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Max_Cool_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MAX_COOL_SETPOINT_LIMIT);
//        SyncDeviceAction(DeviceInfo::DI_Daikin_Control_Seq_Operation,    ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_CTRL_SEQ_OF_OPER);
        SyncDeviceAction(DeviceInfo::DI_Daikin_System_Mode,              ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_SYSTEM_MODE);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Fan_Mode,                 ZCL_CLUSTER_ID_HAVC_FAN_CONTROL,        ATTRID_HVAC_FAN_CTRL_FAN_MODE);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Fan_Direction,            ZCL_CLUSTER_ID_HAVC_FAN_CONTROL,        ATTRID_HVAC_FAN_CTRL_FAN_DIRECTION);

        Action[DeviceInfo::DI_Model].DP_DIStringName                          = std::string("model");
        Action[DeviceInfo::DI_Manufacturer].DP_DIStringName                   = std::string("manufacturer");
        Action[DeviceInfo::DI_State].DP_DIStringName                          = std::string("state");
        Action[DeviceInfo::DI_Daikin_Local_Temperature].DP_DIStringName       = std::string("localtemp");
        Action[DeviceInfo::DI_Daikin_Cooling_Setpoint].DP_DIStringName        = std::string("coolingset");
        Action[DeviceInfo::DI_Daikin_Heating_Setpoint].DP_DIStringName        = std::string("heatingset");
        Action[DeviceInfo::DI_Daikin_Min_Heat_Limit].DP_DIStringName          = std::string("minheat");
        Action[DeviceInfo::DI_Daikin_Max_Heat_Limit].DP_DIStringName          = std::string("maxheat");
        Action[DeviceInfo::DI_Daikin_Min_Cool_Limit].DP_DIStringName          = std::string("mincool");
        Action[DeviceInfo::DI_Daikin_Max_Cool_Limit].DP_DIStringName          = std::string("maxcool");
//        Action[DeviceInfo::DI_Daikin_Control_Seq_Operation].DP_DIStringName   = std::string("ctrloper");
        Action[DeviceInfo::DI_Daikin_System_Mode].DP_DIStringName             = std::string("sysmod");
        Action[DeviceInfo::DI_Daikin_Fan_Mode].DP_DIStringName                = std::string("fanmod");
        Action[DeviceInfo::DI_Daikin_Fan_Direction].DP_DIStringName           = std::string("fandiect");

        RealType = LUMI_DEVICE_DAIKIN;
    } else if (prefixModel == String("IR-CMD")) {
        RealType = 99;
    } else {
        RealType = 0; //Other brands!!!
        OtherBrandsDevices();
    }

    for(Action_t::const_iterator_t it = Action.begin(); it != Action.end(); it++) {
        Action[it->first].DP_DIName = it->first;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbDeviceDb::OtherBrandsDevices() {
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbDeviceDb::SyncDeviceAction(
    DeviceInfo  devInfo,
    u16_t       wClusterID,
    u16_t       wAttributeID
) {
    bool_t boRetVal = TRUE;
    Action[devInfo].DP_ClusterID      = wClusterID;
    Action[devInfo].DP_AttributeID    = wAttributeID;

    switch (wClusterID) {
        case ZCL_CLUSTER_ID_GEN_BASIC:
            if ((Action[devInfo].DP_AttributeID         == ATTRID_BASIC_MANUFACTURER_NAME) ||
                    (Action[devInfo].DP_AttributeID     == ATTRID_BASIC_MODEL_ID))
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_CHAR_STR;    GenerateAttributeDataSize(devInfo); break; }
            boRetVal = FALSE;
            break;

        case ZCL_CLUSTER_ID_GEN_POWER_CFG:
            if (Action[devInfo].DP_AttributeID          == ATTRID_POWER_CFG_BATTERY_PERCENTAGE)
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_UINT8;    GenerateAttributeDataSize(devInfo); break; }
            boRetVal = FALSE;
            break;

        case ZCL_CLUSTER_ID_GEN_ON_OFF:
            if (Action[devInfo].DP_AttributeID          == ATTRID_ON_OFF)
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_BOOLEAN;  GenerateAttributeDataSize(devInfo); break; }
            boRetVal = FALSE;
            break;

        case ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL:
            if (Action[devInfo].DP_AttributeID          == ATTRID_LEVEL_CURRENT_LEVEL)
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_UINT8;    GenerateAttributeDataSize(devInfo); break; }
            boRetVal = FALSE;
            break;

        case ZCL_CLUSTER_ID_GEN_BINARY_INPUT:
            if (Action[devInfo].DP_AttributeID          == ATTRID_BINARY_INPUT_PRESENT_VALUE)
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_BOOLEAN; GenerateAttributeDataSize(devInfo); break; }
            boRetVal = FALSE;
            break;

        case ZCL_CLUSTER_ID_CLOSURES_DOOR_CONFIG:
            if (Action[devInfo].DP_AttributeID          == ATTRID_CLOSURES_DOOR_STATE)
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_ENUM8;    GenerateAttributeDataSize(devInfo); break; }
            boRetVal = FALSE;
            break;

        case ZCL_CLUSTER_ID_HAVC_THERMOSTAT:
            if ((Action[devInfo].DP_AttributeID         == ATTRID_HVAC_THERMOSTAT_LOCAL_TEMPERATURE) ||
                    (Action[devInfo].DP_AttributeID     == ATTRID_HVAC_THERMOSTAT_OCCUPIED_COOLING_SETPOINT) ||
                    (Action[devInfo].DP_AttributeID     == ATTRID_HVAC_THERMOSTAT_OCCUPIED_HEATING_SETPOINT) ||
                    (Action[devInfo].DP_AttributeID     == ATTRID_HVAC_THERMOSTAT_MIN_HEAT_SETPOINT_LIMIT)  ||
                    (Action[devInfo].DP_AttributeID     == ATTRID_HVAC_THERMOSTAT_MAX_HEAT_SETPOINT_LIMIT)  ||
                    (Action[devInfo].DP_AttributeID     == ATTRID_HVAC_THERMOSTAT_MIN_COOL_SETPOINT_LIMIT)  ||
                    (Action[devInfo].DP_AttributeID     == ATTRID_HVAC_THERMOSTAT_MAX_COOL_SETPOINT_LIMIT))
            { Action[devInfo].DP_AttributeDataType      = ZCL_DATATYPE_INT16;   GenerateAttributeDataSize(devInfo); break; }

            if ((Action[devInfo].DP_AttributeID         == ATTRID_HVAC_THERMOSTAT_CTRL_SEQ_OF_OPER) ||
                    (Action[devInfo].DP_AttributeID     == ATTRID_HVAC_THERMOSTAT_SYSTEM_MODE))
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_ENUM8;   GenerateAttributeDataSize(devInfo); break; }

            boRetVal = FALSE;
            break;

        case ZCL_CLUSTER_ID_HAVC_FAN_CONTROL:
            if ((Action[devInfo].DP_AttributeID         == ATTRID_HVAC_FAN_CTRL_FAN_MODE) ||
                    (Action[devInfo].DP_AttributeID     == ATTRID_HVAC_FAN_CTRL_FAN_DIRECTION))
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_ENUM8;   GenerateAttributeDataSize(devInfo); break; }
            boRetVal = FALSE;
            break;

        case ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL:
            if (Action[devInfo].DP_AttributeID          == ATTRID_LIGHTING_COLOR_CONTROL_REMAINING_TIME)
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_UINT16;  GenerateAttributeDataSize(devInfo); break; }
            if ((Action[devInfo].DP_AttributeID         == ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_R_INTENSITY) ||
                    (Action[devInfo].DP_AttributeID     == ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_G_INTENSITY) ||
                    (Action[devInfo].DP_AttributeID     == ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_B_INTENSITY))
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_UINT8;  GenerateAttributeDataSize(devInfo); break; }
            boRetVal = FALSE;
            break;

        case ZCL_CLUSTER_ID_SS_IAS_ZONE:
            if (Action[devInfo].DP_AttributeID          == ATTRID_SS_IAS_ZONE_STATUS)
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_BITMAP16; GenerateAttributeDataSize(devInfo); break; }
            boRetVal = FALSE;
            break;

        case ZCL_CLUSTER_ID_LUMI_IR:
            if (Action[devInfo].DP_AttributeID          == ATTRID_LUMI_IR)
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_BITMAP24;   GenerateAttributeDataSize(devInfo); break; }
            if (Action[devInfo].DP_AttributeID          == ATTRID_SS_IAS_ZONE_STATUS)
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_ENUM16;   GenerateAttributeDataSize(devInfo); break; }
            boRetVal = FALSE;
            break;

        default:
            Action[devInfo].DP_AttributeDataType        = ZCL_DATATYPE_UNKNOWN;
            Action[devInfo].DP_AttributeDataType        = 1;//Value 1 to avoid memory allocating at ReadAttributeResponse.
            boRetVal = FALSE;
            break;
    }
    return !boRetVal;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZbDeviceDb::GetAttributeDataSize(
    u8_t  byAttributeDataType,
    u8_p* byInBuffer
){
    u8_t byRetVal;
    switch (byAttributeDataType) {
        case ZCL_DATATYPE_DATA8:
        case ZCL_DATATYPE_BOOLEAN:
        case ZCL_DATATYPE_BITMAP8:
        case ZCL_DATATYPE_UINT8:
        case ZCL_DATATYPE_INT8:
        case ZCL_DATATYPE_ENUM8:
            byRetVal = 1;
            break;

        case ZCL_DATATYPE_DATA16:
        case ZCL_DATATYPE_BITMAP16:
        case ZCL_DATATYPE_UINT16:
        case ZCL_DATATYPE_INT16:
        case ZCL_DATATYPE_ENUM16:
        case ZCL_DATATYPE_SEMI_PREC:
        case ZCL_DATATYPE_CLUSTER_ID:
        case ZCL_DATATYPE_ATTR_ID:
            byRetVal = 2;
            break;

        case ZCL_DATATYPE_DATA24:
        case ZCL_DATATYPE_BITMAP24:
        case ZCL_DATATYPE_UINT24:
        case ZCL_DATATYPE_INT24:
            byRetVal = 3;
            break;

        case ZCL_DATATYPE_DATA32:
        case ZCL_DATATYPE_BITMAP32:
        case ZCL_DATATYPE_UINT32:
        case ZCL_DATATYPE_INT32:
        case ZCL_DATATYPE_SINGLE_PREC:
        case ZCL_DATATYPE_TOD:
        case ZCL_DATATYPE_DATE:
        case ZCL_DATATYPE_UTC:
        case ZCL_DATATYPE_BAC_OID:
            byRetVal = 4;
            break;

        case ZCL_DATATYPE_DATA40:
        case ZCL_DATATYPE_BITMAP40:
        case ZCL_DATATYPE_UINT40:
        case ZCL_DATATYPE_INT40:
            byRetVal = 5;
            break;

        case ZCL_DATATYPE_DATA48:
        case ZCL_DATATYPE_BITMAP48:
        case ZCL_DATATYPE_UINT48:
        case ZCL_DATATYPE_INT48:
            byRetVal = 6;
            break;

        case ZCL_DATATYPE_DATA56:
        case ZCL_DATATYPE_BITMAP56:
        case ZCL_DATATYPE_UINT56:
        case ZCL_DATATYPE_INT56:
            byRetVal = 7;
            break;

        case ZCL_DATATYPE_DATA64:
        case ZCL_DATATYPE_BITMAP64:
        case ZCL_DATATYPE_UINT64:
        case ZCL_DATATYPE_INT64:
        case ZCL_DATATYPE_DOUBLE_PREC:
        case ZCL_DATATYPE_IEEE_ADDR:
            byRetVal = 8;
            break;

        case ZCL_DATATYPE_128_SECURITY_KEY:
            byRetVal = 16;
            break;

        default:
            byRetVal = 0;
            break;
    }

    if ((byRetVal == 0) && (byInBuffer != NULL)) {
        switch (byAttributeDataType) {
            case 0x41: //Octet string, Character string
            case 0x42:
                byRetVal = **byInBuffer;
                ++*byInBuffer;
                break;

            case 0x43: //Long octet string, Long character string
            case 0x44:
                byRetVal = **byInBuffer | **(byInBuffer + 1) << 8;
                *byInBuffer += 2;
                break;

            default:
                byRetVal = 0;
                break;
        }
    }

    return byRetVal;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDeviceDb::GenerateAttributeDataSize(
    DeviceInfo  devInfo
){
    Action[devInfo].DP_AttributeDataSize = GetAttributeDataSize(Action[devInfo].DP_AttributeDataType, NULL);
}
