/*
 * JsonDevStt.hpp
 *
 *  Created on: Nov 7, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONDEVSTT_HPP_
#define CONTROLLER_JSONMESSAGE_JSONDEVSTT_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "ZbModelDb.hpp"

class JsonRGBStt {
public:
    JsonRGBStt() {}
    virtual ~JsonRGBStt() {}
    JsonCommand_p CreateJsonCommand(ZbDeviceDb_p device);
    static String GetStrCmd() { return "rgb=res"; }
};

typedef JsonRGBStt  JsonRGBRes_t;
typedef JsonRGBStt* JsonRGBRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonRGBStt::CreateJsonCommand(
    ZbDeviceDb_p device
) {
    JsonCommand_p pJsonCommand = new JsonCommand("rgb", "res");
    Json::Value jsonValue;
    jsonValue["devid"] = std::to_string(device->DeviceID.GetValue());
    jsonValue["red"] = std::to_string(device->Action[DI_RGB_Red].DP_AttributeData);
    jsonValue["green"] = std::to_string(device->Action[DI_RGB_Green].DP_AttributeData);
    jsonValue["blue"] = std::to_string(device->Action[DI_RGB_Blue].DP_AttributeData);
    jsonValue["time"] = std::to_string(device->Action[DI_RGB_RemainingTime].DP_AttributeData);

    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONDEVSTT_HPP_ */
