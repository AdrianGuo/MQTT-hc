/*
 * JsonDaikinSet.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINSET_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINSET_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "DeviceInfo.hpp"
#include "Vector.hpp"

class JsonDaikinSet {
private:
    Vector<Action_t> m_vDeviceAction;
    virtual bool_t ParseJsonValue(Json::Value& jArray);
public:
    JsonDaikinSet() {}
    virtual ~JsonDaikinSet() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "daikin=set"; }
    Vector<Action_t> Return() const { return m_vDeviceAction; }
};

typedef JsonDaikinSet  JsonDaikinSet_t;
typedef JsonDaikinSet* JsonDaikinSet_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDaikinSet::ParseJsonCommand(
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
JsonDaikinSet::ParseJsonValue(
    Json::Value& jsonValue
) {
    int_t i = 0;

    if(!jsonValue.isMember("devid") || !jsonValue.isMember("info")) { return FALSE; }

    const Json::Value& jArray = jsonValue["info"];

    if (jArray.isMember("state"))        { i++; Action_t temp; temp[DeviceInfo::DI_State] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (jArray.isMember("localtemp"))    { i++; Action_t temp; temp[DeviceInfo::DI_Daikin_Local_Temperature] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (jArray.isMember("minheat"))      { i++; Action_t temp; temp[DeviceInfo::DI_Daikin_Min_Heat_Limit] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (jArray.isMember("maxheat"))      { i++; Action_t temp; temp[DeviceInfo::DI_Daikin_Max_Heat_Limit] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (jArray.isMember("mincool"))      { i++; Action_t temp; temp[DeviceInfo::DI_Daikin_Min_Cool_Limit] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (jArray.isMember("maxcool"))      { i++; Action_t temp; temp[DeviceInfo::DI_Daikin_Max_Cool_Limit] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (jArray.isMember("coolingset"))   { i++; Action_t temp; temp[DeviceInfo::DI_Daikin_Cooling_Setpoint] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (jArray.isMember("heatingset"))   { i++; Action_t temp; temp[DeviceInfo::DI_Daikin_Heating_Setpoint] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (jArray.isMember("ctrloper"))     { i++; Action_t temp; temp[DeviceInfo::DI_Daikin_Control_Seq_Operation] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (jArray.isMember("sysmod"))       { i++; Action_t temp; temp[DeviceInfo::DI_Daikin_System_Mode] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (jArray.isMember("fanmod"))       { i++; Action_t temp; temp[DeviceInfo::DI_Daikin_Fan_Mode] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (jArray.isMember("fandiect"))     { i++; Action_t temp; temp[DeviceInfo::DI_Daikin_Fan_Direction] = atoi(jArray[].asCString());  m_vDeviceAction.push_back(temp); }

    if (i == 0) { return FALSE; }

    return TRUE;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINSET_HPP_ */
