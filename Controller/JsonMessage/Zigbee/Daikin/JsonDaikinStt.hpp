/*
 * JsonDaikinStt.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINSTT_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINSTT_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "ZbModelDb.hpp"

class JsonDaikinStt {
public:
    JsonDaikinStt() {}
    virtual ~JsonDaikinStt() {}
    JsonCommand_p CreateJsonCommand(Device_t device);
    static String GetStrCmd() { return "daikin=stt"; }
};

typedef JsonDaikinStt  JsonDaikinStt_t;
typedef JsonDaikinStt* JsonDaikinStt_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonDaikinStt::CreateJsonCommand(
    Device_t device
) {
    JsonCommand_p pJsonCommand = new JsonCommand("daikin", "stt");
    Json::Value jsonValue, jsonInfo;
    jsonValue["devid"] = device->DeviceID.GetValue();
    jsonInfo[device.Modify()->Action[DeviceInfo::DI_Using].DP_DIStringName] = std::to_string(device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeData);
    jsonValue["info"].append(jsonInfo);
    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINSTT_HPP_ */
