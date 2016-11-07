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
#include <ZbDeviceControl.hpp>

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
    ControllerID    (0,  "ControllerID") {

//    State       =   Action[DeviceInfo::DI_State].DP_AttributeData;
    RealType    =   0;
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
    switch (Type) {
    case ZCL_HA_DEVICEID_ON_OFF_LIGHT:
    case ZCL_HA_DEVICEID_DIMMABLE_LIGHT:
    case ZCL_HA_DEVICEID_LIGHT_SENSOR:
    case ZCL_HA_DEVICEID_TEMPERATURE_SENSOR:
    case ZCL_HA_DEVICEID_THERMOSTAT:
    case ZCL_HA_DEVICEID_IAS_ZONE:
    case ZCL_HA_DEVICEID_COLORED_DIMMABLE_LIGHT:
    case ZCL_HA_DEVICEID_HEATING_COOLING_UNIT:
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
    u8_t byAttributeDataType,
    u8_p const pbyValue
){
    DEBUG1("Received information from device.")
    switch (RealType) {
        case LUMI_DEVICE_SWITCH:
        case LUMI_DEVICE_INPUT:
            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_State]) break;
            State = (u8_t) *pbyValue;
            ForwardDeviceInfoToOutside(this);
            break;

        case LUMI_DEVICE_DIMMER:
            if (Action[DeviceInfo::DI_Using] == Action[DeviceInfo::DI_State]) {
                State = (u8_t) *pbyValue;
            }
            SendDimmersStateToOutside(this);

            if (Action[DeviceInfo::DI_Using] == Action[DeviceInfo::DI_OnOff]) {
                State = (u8_t) *pbyValue;
            }
            break;

        case LUMI_DEVICE_FAN:
            if (Action[DeviceInfo::DI_Using] == Action[DeviceInfo::DI_State]) {
                State = (u8_t) *pbyValue;
            }
            SendFansStateToOutside(this);

            if (Action[DeviceInfo::DI_Using] == Action[DeviceInfo::DI_OnOff]) {
                State = (u8_t) *pbyValue;
            }
            break;

        case LUMI_DEVICE_CURTAIN:
            if (Action[DeviceInfo::DI_Using] == Action[DeviceInfo::DI_State]) {
                State = (u8_t) *pbyValue;
            }

            if (Action[DeviceInfo::DI_Using] == Action[DeviceInfo::DI_OnOff]) {
                State = (u8_t) *pbyValue;
            }
            break;

        case LUMI_DEVICE_IR:
            if (Action[DeviceInfo::DI_Using] == Action[DeviceInfo::DI_State])
                //respone from ???
            break;

        case LUMI_DEVICE_DOOR:
            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_State])
                State = (u8_t) *pbyValue;
            ForwardDeviceInfoToOutside(this);

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Power])
                Action[DeviceInfo::DI_Power].DP_AttributeData = (u8_t) *pbyValue;
            break;

        case LUMI_DEVICE_PIR:
        case LUMI_DEVICE_TEMPERATURE:
        case LUMI_DEVICE_HUMIDITY:
        case LUMI_DEVICE_ILLUMINANCE:
            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_State])
                State = (u16_t) *pbyValue;
            ForwardDeviceInfoToOutside(this);

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Power])
                Action[DeviceInfo::DI_Power].DP_AttributeData = (u8_t) *pbyValue;
            break;

        case LUMI_DEVICE_RGB:
            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_State])
                State = (u8_t) *pbyValue; //ON/OFF state

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_RGB_RemainingTime])
                Action[DeviceInfo::DI_RGB_RemainingTime].DP_AttributeData = (u16_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_RGB_Red])
                Action[DeviceInfo::DI_RGB_Red].DP_AttributeData = (u8_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_RGB_Green])
                Action[DeviceInfo::DI_RGB_Green].DP_AttributeData = (u8_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_RGB_Blue])
                Action[DeviceInfo::DI_RGB_Blue].DP_AttributeData = (u8_t) *pbyValue;
            SendRGBsStateToOutside(this);
            break;

        case LUMI_DEVICE_DAIKIN:
            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_State])
                State = (u16_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Daikin_Local_Temperature])
                Action[DeviceInfo::DI_Daikin_Local_Temperature].DP_AttributeData = (u16_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Daikin_Cooling_Setpoint])
                Action[DeviceInfo::DI_Daikin_Cooling_Setpoint].DP_AttributeData = (u16_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Daikin_Heating_Setpoint])
                Action[DeviceInfo::DI_Daikin_Heating_Setpoint].DP_AttributeData = (u16_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Daikin_Min_Heat_Limit])
                Action[DeviceInfo::DI_Daikin_Min_Heat_Limit].DP_AttributeData = (u16_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Daikin_Max_Heat_Limit])
                Action[DeviceInfo::DI_Daikin_Max_Heat_Limit].DP_AttributeData = (u16_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Daikin_Min_Cool_Limit])
                Action[DeviceInfo::DI_Daikin_Min_Cool_Limit].DP_AttributeData = (u16_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Daikin_Max_Cool_Limit])
                Action[DeviceInfo::DI_Daikin_Max_Cool_Limit].DP_AttributeData = (u16_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Daikin_Control_Seq_Operation])
                Action[DeviceInfo::DI_Daikin_Control_Seq_Operation].DP_AttributeData = (u8_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Daikin_System_Mode])
                Action[DeviceInfo::DI_Daikin_System_Mode].DP_AttributeData = (u8_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Daikin_Fan_Mode])
                Action[DeviceInfo::DI_Daikin_Fan_Mode].DP_AttributeData = (u8_t) *pbyValue;

            if (Action[DeviceInfo::DI_Using] != Action[DeviceInfo::DI_Daikin_Fan_Direction])
                Action[DeviceInfo::DI_Daikin_Fan_Direction].DP_AttributeData = (u8_t) *pbyValue;

            break;

        default:
            break;
    }

    m_pLocker->Lock();
    this->State = (int_t) *pbyValue;
    m_pLocker->UnLock();

//    ZbSocketCmd::_instance->ReportDevicesState(1, pDevice);
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDeviceDb::EnvAttached() {
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
    SyncDeviceAction(DeviceInfo::DI_Power,       ZCL_CLUSTER_ID_GEN_POWER_CFG,                 ATTRID_POWER_CFG_BATTERY_PERCENTAGE);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDeviceDb::GenerateDeviceInfo() {
//    DEBUG2("________Model________: %s", ((String ) Model).c_str());

    if ((Model == String("LM-SZ1")) || (Model == String("LM-SZ2")) || (Model == String("LM-SZ3")) || (Model == String("LM-SZ4")) || (Model == String("LM-SZ10"))){
        SyncDeviceAction(DeviceInfo::DI_State,       ZCL_CLUSTER_ID_GEN_ON_OFF, ATTRID_ON_OFF);
        RealType = LUMI_DEVICE_SWITCH;
    } else if ((Model == String("LM-DZ1")) || (Model == String("LM-DZ2")) ) {
        SyncDeviceAction(DeviceInfo::DI_State,       ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL,   ATTRID_LEVEL_CURRENT_LEVEL);
        SyncDeviceAction(DeviceInfo::DI_OnOff,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);
        RealType = LUMI_DEVICE_DIMMER;
    } else if (Model == String("LM-FZ1")) {
        SyncDeviceAction(DeviceInfo::DI_State,       ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL,   ATTRID_LEVEL_CURRENT_LEVEL);
        SyncDeviceAction(DeviceInfo::DI_OnOff,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);
        RealType = LUMI_DEVICE_FAN;
    } else if ((Model == String("LM-BZ1")) || (Model == String("LM-BZ2"))) {
        SyncDeviceAction(DeviceInfo::DI_State,       ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL,   ATTRID_LEVEL_CURRENT_LEVEL);
        SyncDeviceAction(DeviceInfo::DI_OnOff,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);
        RealType = LUMI_DEVICE_CURTAIN;
    } else if (Model == String("LM-IR")) {
        SyncDeviceAction(DeviceInfo::DI_State,       ZCL_CLUSTER_ID_LUMI_IR,             ATTRID_LUMI_IR);
        SyncDeviceAction(DeviceInfo::DI_OnOff,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);
        RealType = LUMI_DEVICE_IR;
    } else if (Model == String("LM-PIR")) {
        if (Type == ZCL_HA_DEVICEID_IAS_ZONE) {
            SyncDeviceAction(DeviceInfo::DI_State,   ZCL_CLUSTER_ID_SS_IAS_ZONE,         ATTRID_SS_IAS_ZONE_STATUS);
            SyncDeviceAction(DeviceInfo::DI_Power,   ZCL_CLUSTER_ID_GEN_POWER_CFG,       ATTRID_POWER_CFG_BATTERY_PERCENTAGE);
            RealType = LUMI_DEVICE_PIR;
        } else {
            EnvAttached();
        }
    } else if (Model == String("LM-DOOR")) {
        if (Type == ZCL_HA_DEVICEID_DOOR_LOCK) {
            SyncDeviceAction(DeviceInfo::DI_State,   ZCL_CLUSTER_ID_CLOSURES_DOOR_CONFIG, ATTRID_CLOSURES_DOOR_STATE);
            SyncDeviceAction(DeviceInfo::DI_Power,   ZCL_CLUSTER_ID_GEN_POWER_CFG,       ATTRID_POWER_CFG_BATTERY_PERCENTAGE);
            RealType = LUMI_DEVICE_DOOR;
        } else {
            EnvAttached();
        }
    } else if (Model == String("LM-ENVR")) {
        EnvAttached();
    } else if (Model == String("LM-RGB")) {
        SyncDeviceAction(DeviceInfo::DI_State,                           ZCL_CLUSTER_ID_GEN_ON_OFF,              ATTRID_ON_OFF);
        SyncDeviceAction(DeviceInfo::DI_RGB_RemainingTime,               ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_REMAINING_TIME);
        SyncDeviceAction(DeviceInfo::DI_RGB_Red,                         ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_R_INTENSITY);
        SyncDeviceAction(DeviceInfo::DI_RGB_Green,                       ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_G_INTENSITY);
        SyncDeviceAction(DeviceInfo::DI_RGB_Blue,                        ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_B_INTENSITY);
        RealType = LUMI_DEVICE_RGB;
    } else if(Model == String("LM-INPUT")) {
        SyncDeviceAction(DeviceInfo::DI_State,                           ZCL_CLUSTER_ID_GEN_BINARY_INPUT,        ATTRID_BINARY_INPUT_PRESENT_VALUE);
        RealType = LUMI_DEVICE_INPUT;
    } else if (Model == String("LM-DKZ")) {
        SyncDeviceAction(DeviceInfo::DI_State,                           ZCL_CLUSTER_ID_GEN_ON_OFF,              ATTRID_ON_OFF);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Local_Temperature,        ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_LOCAL_TEMPERATURE);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Cooling_Setpoint,         ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_OCCUPIED_COOLING_SETPOINT);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Heating_Setpoint,         ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_OCCUPIED_HEATING_SETPOINT);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Min_Heat_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MIN_HEAT_SETPOINT_LIMIT);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Max_Heat_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MAX_HEAT_SETPOINT_LIMIT);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Min_Cool_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MIN_COOL_SETPOINT_LIMIT);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Max_Cool_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MAX_COOL_SETPOINT_LIMIT);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Control_Seq_Operation,    ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_CTRL_SEQ_OF_OPER);
        SyncDeviceAction(DeviceInfo::DI_Daikin_System_Mode,              ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_SYSTEM_MODE);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Fan_Mode,                 ZCL_CLUSTER_ID_HAVC_FAN_CONTROL,        ATTRID_HVAC_FAN_CTRL_FAN_MODE);
        SyncDeviceAction(DeviceInfo::DI_Daikin_Fan_Direction,            ZCL_CLUSTER_ID_HAVC_FAN_CONTROL,        ATTRID_HVAC_FAN_CTRL_FAN_DIRECTION);
        RealType = LUMI_DEVICE_DAIKIN;
    } else {
        RealType = 0; //Other brands!!!
        OtherBrandsDevices();
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
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_ENUM8;   GenerateAttributeDataSize(devInfo); break; }
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
bool_t
ZbDeviceDb::SyncDeviceAction() {
    return SyncDeviceAction(DeviceInfo::DI_Using, Action[DeviceInfo::DI_Using].DP_ClusterID, Action[DeviceInfo::DI_Using].DP_AttributeID);
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
