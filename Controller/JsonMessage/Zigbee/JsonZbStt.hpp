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
    JsonCommand_p CreateJsonCommand(Devices_t devices);
    static String GetStrCmd() { return "zb=stt"; }
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
    Devices_t devices
) {
    JsonCommand_p pJsonCommand = new JsonCommand("zb", "stt");
    Json::Value jsonValue;
    for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        Json::Value dev;
        dev["devid"] = std::to_string((*it)->DeviceID.GetValue());
        dev["ord"] = std::to_string((*it)->Endpoint.GetValue());
        dev["val"] = std::to_string((*it)->State);
        jsonValue["dev"].append(dev);
    }

    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBSTT_HPP_ */
