/*
 * JsonRGBEnableRes.hpp
 *
 *  Created on: Nov 7, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_JSONRGBENABLERES_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_JSONRGBENABLERES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRGBEnaRes {
public:
    JsonRGBEnaRes() {}
    virtual ~JsonRGBEnaRes() {}
    JsonCommand_p CreateJsonCommand(int_t devid, u8_t byValue);
    static String GetStrCmd() { return "rgb=enares"; }
};

typedef JsonRGBEnaRes  JsonRGBEnaRes_t;
typedef JsonRGBEnaRes* JsonRGBEnaRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonRGBEnaRes::CreateJsonCommand(
    int_t devid,
    u8_t byValue
) {
    JsonCommand_p pJsonCommand = new JsonCommand("rgb", "enares");
    Json::Value jsonValue;
    jsonValue["devid"] = std::to_string(devid);
    jsonValue["ret"] = std::to_string(byValue);

    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_JSONRGBENABLERES_HPP_ */
