/*
 * JsonDaikinGet.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINGET_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINGET_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "DeviceInfo.hpp"
#include "Vector.hpp"

class JsonDaikinGet {
private:
    Vector<DeviceInfo> m_vDeviceInfo;
    virtual bool_t ParseJsonValue(Json::Value& jArray);
public:
    JsonDaikinGet() {}
    virtual ~JsonDaikinGet() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "daikin=get"; }
    Vector<DeviceInfo> Return() const { return m_vDeviceInfo; }
};

typedef JsonDaikinGet  JsonDaikinGet_t;
typedef JsonDaikinGet* JsonDaikinGet_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDaikinGet::ParseJsonCommand(
    JsonCommand_p pJsonComand
) {
    return ParseJsonValue(pJsonComand->GetJsonOjbect());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDaikinGet::ParseJsonValue(
    Json::Value& jsonValue
) {
    int_t i = 0;

    if(!jsonValue.isMember("devid") || !jsonValue.isMember("info")) { return FALSE; }

    const Json::Value& jArray = jsonValue["info"];

    if (jArray.isMember("state"))        { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_State); }

    if (jArray.isMember("localtemp"))    { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_Daikin_Local_Temperature); }

    if (jArray.isMember("minheat"))      { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_Daikin_Min_Heat_Limit); }

    if (jArray.isMember("maxheat"))      { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_Daikin_Max_Heat_Limit); }

    if (jArray.isMember("mincool"))      { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_Daikin_Min_Cool_Limit); }

    if (jArray.isMember("maxcool"))      { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_Daikin_Max_Cool_Limit); }

    if (jArray.isMember("coolingset"))   { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_Daikin_Cooling_Setpoint); }

    if (jArray.isMember("heatingset"))   { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_Daikin_Heating_Setpoint); }

    if (jArray.isMember("ctrloper"))     { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_Daikin_Control_Seq_Operation); }

    if (jArray.isMember("sysmod"))       { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_Daikin_System_Mode); }

    if (jArray.isMember("fanmod"))       { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_Daikin_Fan_Mode); }

    if (jArray.isMember("fandiect"))     { i++; m_vDeviceInfo.push_back(DeviceInfo::DI_Daikin_Fan_Direction); }

    if (i == 0) { return FALSE; }

    return TRUE;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINGET_HPP_ */
