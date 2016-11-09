/*
 * JsonRGBRes.hpp
 *
 *  Created on: Nov 7, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_JSONRGBRES_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_JSONRGBRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "ZbModelDb.hpp"

class JsonRGBStt {
public:
    JsonRGBStt() {}
    virtual ~JsonRGBStt() {}
    JsonCommand_p CreateJsonCommand(Device_t device);
    static String GetStrCmd() { return "rgb=stt"; }
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
    Device_t device
) {
    JsonCommand_p pJsonCommand = new JsonCommand("rgb", "stt");
    Json::Value jsonValue;
    jsonValue["devid"] = std::to_string(device->DeviceID.GetValue());
    jsonValue["val"] = std::to_string(device.Modify()->Action[DI_RGB_Red].DP_AttributeData) + ":"
            + std::to_string(device.Modify()->Action[DI_RGB_Green].DP_AttributeData) + ":"
            + std::to_string(device.Modify()->Action[DI_RGB_Blue].DP_AttributeData) + ":"
            + std::to_string(device.Modify()->Action[DI_RGB_RemainingTime].DP_AttributeData);

    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_JSONRGBRES_HPP_ */
