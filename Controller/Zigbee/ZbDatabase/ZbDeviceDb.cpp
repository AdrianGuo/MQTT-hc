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
    State           (Action[DI_State].DP_AttributeData)
{
    RealType    =   0;
    m_pLocker   =   new Locker();

    SyncDeviceAction(DI_Model,           ZCL_CLUSTER_ID_GEN_BASIC, ATTRID_BASIC_MODEL_ID);
    SyncDeviceAction(DI_Manufacturer,    ZCL_CLUSTER_ID_GEN_BASIC, ATTRID_BASIC_MANUFACTURER_NAME);
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
ZbDeviceDb::IsInterested(int_t iType) {
    bool_t boRet = FALSE;
    switch (iType) {
        case ZCL_HA_DEVICEID_ON_OFF_LIGHT:
        case ZCL_HA_DEVICEID_DIMMABLE_LIGHT:
        case ZCL_HA_DEVICEID_LIGHT_SENSOR:
        case ZCL_HA_DEVICEID_TEMPERATURE_SENSOR:
        case ZCL_HA_DEVICEID_THERMOSTAT:
        case ZCL_HA_DEVICEID_IAS_ZONE:
        case ZCL_HA_DEVICEID_COLORED_DIMMABLE_LIGHT:
        case ZCL_HA_DEVICEID_HEATING_COOLING_UNIT:
        case ZCL_HA_DEVICEID_SHADE:
        case ZCL_HA_DEVICEID_DOOR_LOCK:
        case ZCL_HA_DEVICEID_SIMPLE_INPUT:
        case ZCL_LUMI_DEVICEID_IR:
        case ZCL_LUMI_DEVICEID_POWER:
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
bool_t
ZbDeviceDb::IsInterested() {
 return IsInterested(Type.GetValue());
}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDeviceDb::ReceiveInforFromDevice(
    DeviceProperties vDP,
    Vector<u8_p> vpData
){
    u8_t byLimit = vDP.size();

    switch (RealType) {
/*
 *  LUMI_DEVICE_SWITCH, LUMI_DEVICE_INPUT
 */
        case LUMI_DEVICE_SWITCH:
        case LUMI_DEVICE_INPUT: {
            for(u8_t i = 0; i < byLimit; i++) {
                Backup(vDP[i].DP_DIName);
                AttributeData(vDP[i].DP_DIName) = *vpData[i];
                ForwardStateToOutside(this);
            }
        }
            break;
/*
 * LUMI_DEVICE_DIMMER, LUMI_DEVICE_CURTAIN, LUMI_DEVICE_FAN
 */
        case LUMI_DEVICE_DIMMER:
        case LUMI_DEVICE_CURTAIN:
        case LUMI_DEVICE_FAN: {
            for(u8_t i = 0; i < byLimit; i++) {
                Backup(vDP[i].DP_DIName);
                if(vDP[i].DP_DIName == DI_State) {
                    if(((PendingReqs(DI_State).front().ReqValue - *vpData[i]) > 5) ||
                            ((PendingReqs(DI_State).front().ReqValue - *vpData[i]) < -5)) {
                                AttributeData(vDP[i].DP_DIName) = *vpData[i];
                            } else {
                                AttributeData(vDP[i].DP_DIName) = PendingReqs(DI_State).front().ReqValue;
                            }
                } else {
                    AttributeData(vDP[i].DP_DIName) = *vpData[i];
                }
                IsResponsed(vDP[i].DP_DIName) = TRUE;
                PopReq(vDP[i].DP_DIName);
            }
            if(IsResponsed(DI_State) && IsResponsed(DI_OnOff)) {
                if(RealType == LUMI_DEVICE_FAN) ForwardFanStateToOutside(this);
                else if (RealType == LUMI_DEVICE_CURTAIN) ForwardCurtainStateToOutside(this);
                else ForwardDimmerStateToOutside(this);

                IsResponsed(DI_State) = FALSE;
                IsResponsed(DI_OnOff) = FALSE;
            }
        }
            break;
/*
 * LUMI_DEVICE_IR
 */
        case LUMI_DEVICE_IR: {
            for(u8_t i = 0; i < byLimit; i++) {
                Backup(vDP[i].DP_DIName);
                if (vDP[i].DP_DIName == DI_IR_Data) {
                    u8_p pbyData = vpData[i];
                    State = *pbyData;
                    ++pbyData;
                    ReserveData(DI_State) = LittleWord(&pbyData, false);
                    IsResponsed(vDP[i].DP_DIName) = TRUE;
                    ForwardIrState(this);
                    pbyData = NULL;
                    IsResponsed(vDP[i].DP_DIName) = FALSE;
                }
                PopReq(vDP[i].DP_DIName);
            }
        }
            break;
/*
 * LUMI_DEVICE_DOOR, LUMI_DEVICE_PIR, LUMI_DEVICE_POWER
 * LUMI_DEVICE_TEMPERATURE, LUMI_DEVICE_HUMIDITY, LUMI_DEVICE_ILLUMINANCE
 */
        case LUMI_DEVICE_DOOR:
        case LUMI_DEVICE_PIR:
        case LUMI_DEVICE_POWER:
        case LUMI_DEVICE_TEMPERATURE:
        case LUMI_DEVICE_HUMIDITY:
        case LUMI_DEVICE_ILLUMINANCE: {
            for(u8_t i = 0; i < byLimit; i++) {
                Backup(vDP[i].DP_DIName);
                if (vDP[i].DP_DIName == DI_State) {
                    if(RealType == LUMI_DEVICE_TEMPERATURE) State = *((i16_p) vpData[i]);
                    else if((RealType == LUMI_DEVICE_PIR) || (RealType == LUMI_DEVICE_HUMIDITY) || (RealType == LUMI_DEVICE_ILLUMINANCE)) State = *((u16_p) vpData[i]);
                    else AttributeData(DI_State) = *vpData[i];
                }
                PopReq(vDP[i].DP_DIName);
            }
            ForwardStateToOutside(this);

        }
            break;
/*
 * LUMI_DEVICE_RGB
 */
        case LUMI_DEVICE_RGB: {
            for (u8_t i = 0; i < byLimit; i++) {
                Backup(vDP[i].DP_DIName);
                AttributeData(vDP[i].DP_DIName) = *vpData[i];
                IsResponsed(vDP[i].DP_DIName) = TRUE;
                PopReq(vDP[i].DP_DIName);
            }
            if (IsResponsed(DI_RGB_Red) &&
                    IsResponsed(DI_RGB_Green) &&
                    IsResponsed(DI_RGB_Blue)) {
                IsResponsed(DI_RGB_Red)     = FALSE;
                IsResponsed(DI_RGB_Green)   = FALSE;
                IsResponsed(DI_RGB_Blue)    = FALSE;
                ForwardRGBStateToOutside(this);
            }
        }
            break;
/*
 * LUMI_DEVICE_DAIKIN
 */
        case LUMI_DEVICE_DAIKIN: {
            for(u8_t i = 0; i < byLimit; i++) {
                Backup(vDP[i].DP_DIName);
                if ((vDP[i].DP_DIName == DI_Daikin_Control_Seq_Operation) ||
                        (vDP[i].DP_DIName == DI_Daikin_System_Mode) ||
                        (vDP[i].DP_DIName == DI_Daikin_Fan_Mode) ||
                        (vDP[i].DP_DIName == DI_Daikin_Fan_Direction))
                {
                    AttributeData(vDP[i].DP_DIName) = *vpData[i];
                } else {
                    AttributeData(vDP[i].DP_DIName) = *((i16_p) vpData[i]);
                }
                IsResponsed(vDP[i].DP_DIName) = TRUE;
//                if(AttributeData(vDP[i].DP_DIName) != PreValue(vDP[i].DP_DIName)) {
//                    IsResponsed(vDP[i].DP_DIName) = TRUE;
//                }

                IsRequested(vDP[i].DP_DIName) = FALSE;
                PopReq(vDP[i].DP_DIName);
            }

            bool_t boCheck = TRUE;
            for(Action_t::const_iterator it = Action.begin(); it != Action.end(); it++) {
                if(it->second.DP_IsRequested == TRUE) {
                    boCheck = FALSE;
                    break;
                }
            }
            if(boCheck || (AttributeData(DI_State) == 0)){
                ForwardDaikinStateToOutside(this);
            }

        }
            break;
/*
 *
 */
        default:
            //ForwardForErrorProcessing(this);
            break;
    }

    for (u8_t i = 0; i < byLimit; i++) {
        delete[] vpData[i];
    }
    vpData.clear();
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
        SyncDeviceAction(DI_State,   ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT,    ATTRID_MS_TEMPERATURE_MEASURED_VALUE);
        RealType = LUMI_DEVICE_TEMPERATURE;
    } else if (Type == ZCL_HA_DEVICEID_THERMOSTAT) {
        SyncDeviceAction(DI_State,   ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY,          ATTRID_MS_RELATIVE_HUMIDITY_MEASURED_VALUE);
        RealType = LUMI_DEVICE_HUMIDITY;
    } else if (Type == ZCL_HA_DEVICEID_LIGHT_SENSOR) {
        SyncDeviceAction(DI_State,   ZCL_CLUSTER_ID_MS_ILLUMINANCE_MEASUREMENT,    ATTRID_MS_ILLUMINANCE_MEASURED_VALUE);
        RealType = LUMI_DEVICE_ILLUMINANCE;
    } else if (Type == ZCL_LUMI_DEVICEID_POWER) {
        SyncDeviceAction(DI_State,   ZCL_CLUSTER_ID_GEN_POWER_CFG,                 ATTRID_POWER_CFG_BATTERY_PERCENTAGE);
        RealType = LUMI_DEVICE_POWER;
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
    GenerateDeviceInfo(prefixModel);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDeviceDb::GenerateDeviceInfo(
        String prefixModel
) {
    if (prefixModel == String("LM-SZ")){
        SyncDeviceAction(DI_State,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);

        RealType = LUMI_DEVICE_SWITCH;
    } else if (prefixModel == String("LM-DZ")) {
        SyncDeviceAction(DI_State,       ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL,   ATTRID_LEVEL_CURRENT_LEVEL);
        SyncDeviceAction(DI_OnOff,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);

        RealType = LUMI_DEVICE_DIMMER;
    } else if (prefixModel == String("LM-FZ")) {
        SyncDeviceAction(DI_State,       ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL,   ATTRID_LEVEL_CURRENT_LEVEL);
        SyncDeviceAction(DI_OnOff,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);

        RealType = LUMI_DEVICE_FAN;
    } else if (prefixModel == String("LM-BZ")) {
        SyncDeviceAction(DI_State,       ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL,   ATTRID_LEVEL_CURRENT_LEVEL);
        SyncDeviceAction(DI_OnOff,       ZCL_CLUSTER_ID_GEN_ON_OFF,          ATTRID_ON_OFF);

        RealType = LUMI_DEVICE_CURTAIN;
    } else if (prefixModel == String("LM-IR")) {
        SyncDeviceAction(DI_State,         ZCL_CLUSTER_ID_LUMI_IR,           ATTRID_LUMI_IRSTATE);
        SyncDeviceAction(DI_IR_Data,       ZCL_CLUSTER_ID_LUMI_IR,           ATTRID_LUMI_IRCMD);
        RealType = LUMI_DEVICE_IR;
    } else if (prefixModel == String("LM-IPZ")) {
        SyncDeviceAction(DI_State,       ZCL_CLUSTER_ID_GEN_BINARY_INPUT,    ATTRID_BINARY_INPUT_PRESENT_VALUE);

        RealType = LUMI_DEVICE_INPUT;
    }   else if (prefixModel == String("LM-PIR")) {
        if (Type == ZCL_HA_DEVICEID_IAS_ZONE) {
            SyncDeviceAction(DI_State,   ZCL_CLUSTER_ID_SS_IAS_ZONE,         ATTRID_SS_IAS_ZONE_STATUS);
            RealType = LUMI_DEVICE_PIR;
        } else {
            EnvAttached();
        }
    } else if (prefixModel == String("LM-DOOR")) {
        if (Type == ZCL_HA_DEVICEID_DOOR_LOCK) {
            SyncDeviceAction(DI_State,   ZCL_CLUSTER_ID_CLOSURES_DOOR_CONFIG, ATTRID_CLOSURES_DOOR_STATE);
            RealType = LUMI_DEVICE_DOOR;
        } else {
            EnvAttached();
        }
    } else if (prefixModel == String("LM-ENVR")) {
        EnvAttached();
    } else if (prefixModel == String("LM-RGB")) {
        SyncDeviceAction(DI_State,                           ZCL_CLUSTER_ID_GEN_ON_OFF,              ATTRID_ON_OFF);
//        SyncDeviceAction(DI_RGB_RemainingTime,               ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_REMAINING_TIME);
        SyncDeviceAction(DI_RGB_Red,                         ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_R_INTENSITY);
        SyncDeviceAction(DI_RGB_Green,                       ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_G_INTENSITY);
        SyncDeviceAction(DI_RGB_Blue,                        ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,  ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_B_INTENSITY);
        RealType = LUMI_DEVICE_RGB;
    } else if(prefixModel == String("LM-INPUT")) {
        SyncDeviceAction(DI_State,                           ZCL_CLUSTER_ID_GEN_BINARY_INPUT,        ATTRID_BINARY_INPUT_PRESENT_VALUE);
        RealType = LUMI_DEVICE_INPUT;
    } else if (prefixModel == String("LM-DKZ")) {
        SyncDeviceAction(DI_State,                           ZCL_CLUSTER_ID_GEN_ON_OFF,              ATTRID_ON_OFF);
        SyncDeviceAction(DI_Daikin_Local_Temperature,        ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_LOCAL_TEMPERATURE);
        SyncDeviceAction(DI_Daikin_Cooling_Setpoint,         ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_OCCUPIED_COOLING_SETPOINT);
        SyncDeviceAction(DI_Daikin_Heating_Setpoint,         ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_OCCUPIED_HEATING_SETPOINT);
        SyncDeviceAction(DI_Daikin_Min_Heat_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MIN_HEAT_SETPOINT_LIMIT);
        SyncDeviceAction(DI_Daikin_Max_Heat_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MAX_HEAT_SETPOINT_LIMIT);
        SyncDeviceAction(DI_Daikin_Min_Cool_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MIN_COOL_SETPOINT_LIMIT);
        SyncDeviceAction(DI_Daikin_Max_Cool_Limit,           ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_MAX_COOL_SETPOINT_LIMIT);
        SyncDeviceAction(DI_Daikin_System_Mode,              ZCL_CLUSTER_ID_HAVC_THERMOSTAT,         ATTRID_HVAC_THERMOSTAT_SYSTEM_MODE);
        SyncDeviceAction(DI_Daikin_Fan_Mode,                 ZCL_CLUSTER_ID_HAVC_FAN_CONTROL,        ATTRID_HVAC_FAN_CTRL_FAN_MODE);
        SyncDeviceAction(DI_Daikin_Fan_Direction,            ZCL_CLUSTER_ID_HAVC_FAN_CONTROL,        ATTRID_HVAC_FAN_CTRL_FAN_DIRECTION);

        Action[DI_Model].DP_DIStringName                          = std::string("model");
        Action[DI_Manufacturer].DP_DIStringName                   = std::string("manufacturer");
        Action[DI_State].DP_DIStringName                          = std::string("state");
        Action[DI_Daikin_Local_Temperature].DP_DIStringName       = std::string("localtemp");
        Action[DI_Daikin_Cooling_Setpoint].DP_DIStringName        = std::string("coolingset");
        Action[DI_Daikin_Heating_Setpoint].DP_DIStringName        = std::string("heatingset");
        Action[DI_Daikin_Min_Heat_Limit].DP_DIStringName          = std::string("minheat");
        Action[DI_Daikin_Max_Heat_Limit].DP_DIStringName          = std::string("maxheat");
        Action[DI_Daikin_Min_Cool_Limit].DP_DIStringName          = std::string("mincool");
        Action[DI_Daikin_Max_Cool_Limit].DP_DIStringName          = std::string("maxcool");
        Action[DI_Daikin_System_Mode].DP_DIStringName             = std::string("sysmod");
        Action[DI_Daikin_Fan_Mode].DP_DIStringName                = std::string("fanmod");
        Action[DI_Daikin_Fan_Direction].DP_DIStringName           = std::string("fandirect");

        RealType = LUMI_DEVICE_DAIKIN;
    } else {
        if(RealType == OTHER_BRAND_DEVICE) { OtherBrandsDevice(); }
    }

    //Add DeviceInfo name to DP
    for(Action_t::const_iterator it = Action.begin(); it != Action.end(); it++) {
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
ZbDeviceDb::OtherBrandsDevice() {
    DEBUG2("Device %s of %s at address %d.", Model.GetValue().c_str(), Manufacturer.GetValue().c_str(), Network.GetValue());
    bool_t boRetVal = TRUE;
    String prefixModel;
    EPInfo_t EPsInfo = ZbZdoCmd::GetInstance()->GetDeviceLogic()[Network.GetValue()];
    switch (Type.GetValue()) {
        case ZCL_HA_DEVICEID_ON_OFF_LIGHT: {
            bool_t boCheck = FALSE;
            for(Map<u8_t, u16_t>::const_iterator it = EPsInfo.mapType.begin(); it != EPsInfo.mapType.end(); it++) {
                if(it->second == ZCL_HA_DEVICEID_DIMMABLE_LIGHT) boCheck = TRUE;
            }
            if(boCheck) prefixModel = "LM-DZ"; //Curtain is OK. Fan???
            else prefixModel = "LM-SZ";
        }
            break;

        case ZCL_HA_DEVICEID_IAS_ZONE:
            prefixModel = "LM-PIR";
            break;

        case ZCL_HA_DEVICEID_DOOR_LOCK:
            prefixModel = "LM-DOOR";
            break;

        case ZCL_HA_DEVICEID_TEMPERATURE_SENSOR:
        case ZCL_HA_DEVICEID_THERMOSTAT:
        case ZCL_HA_DEVICEID_LIGHT_SENSOR:
            EnvAttached();
            break;

        case ZCL_HA_DEVICEID_COLORED_DIMMABLE_LIGHT:
            prefixModel = "RGB";
            break;

        case ZCL_HA_DEVICEID_SIMPLE_INPUT:
            prefixModel = "LM-IPZ";
            break;

        case ZCL_HA_DEVICEID_HEATING_COOLING_UNIT:
            prefixModel = "LM-DKZ";
            break;

        case ZCL_LUMI_DEVICEID_IR:
            prefixModel = "LM-IR";
            break;

        default:
            prefixModel = "OTHER";
            RealType = UNKNOWN_DEVICE;
            boRetVal = FALSE;
            break;
    }
    if(boRetVal) { GenerateDeviceInfo(prefixModel); }
    return boRetVal;
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
            if (Action[devInfo].DP_AttributeID          == ATTRID_LUMI_IRCMD)
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_BITMAP24;   GenerateAttributeDataSize(devInfo); break; }
            if (Action[devInfo].DP_AttributeID          == ATTRID_LUMI_IRSTATE)
                { Action[devInfo].DP_AttributeDataType  = ZCL_DATATYPE_UINT8;   GenerateAttributeDataSize(devInfo); break; }
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
            case ZCL_DATATYPE_OCTET_STR: //Octet string, Character string
            case ZCL_DATATYPE_CHAR_STR:
                byRetVal = **byInBuffer;
                ++*byInBuffer;
                break;

            case ZCL_DATATYPE_LONG_OCTET_STR: //Long octet string, Long character string
            case ZCL_DATATYPE_LONG_CHAR_STR:
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
