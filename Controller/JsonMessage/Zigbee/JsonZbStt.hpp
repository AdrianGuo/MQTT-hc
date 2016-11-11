/*
 * JsonZbStt.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBSTT_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBSTT_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "ZbModelDb.hpp"

class JsonZbStt {
public:
    JsonZbStt() {}
    virtual ~JsonZbStt() {}
    JsonCommand_p CreateJsonCommand(Device_t device, Json::Value val);
    static String GetStrCmd() { return "dev=stt"; }
};

typedef JsonZbStt  JsonZbStt_t;
typedef JsonZbStt* JsonZbStt_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZbStt::CreateJsonCommand(
    Device_t device,
    Json::Value val
) {
    JsonCommand_p pJsonCommand = new JsonCommand("dev", "stt");
    Json::Value jsonValue;
    Json::Value dev;
    dev["devid"] = std::to_string(device->DeviceID.GetValue());
    dev["ord"] = std::to_string(device->Endpoint.GetValue());
    dev["net"] = std::string("1");
    dev["ord"] = std::to_string(device->RealType);
    dev["val"] = val;
    jsonValue["dev"].append(dev);

    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBSTT_HPP_ */
