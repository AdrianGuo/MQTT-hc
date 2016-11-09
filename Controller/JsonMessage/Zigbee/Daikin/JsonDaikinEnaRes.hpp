/*
 * JsonDaikinEnaRes.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINENARES_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINENARES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDaikinEnaRes {
public:
    JsonDaikinEnaRes() {}
    virtual ~JsonDaikinEnaRes() {}
    JsonCommand_p CreateJsonCommand(int_t devid, u8_t byValue);
    static String GetStrCmd() { return "daikin=enares"; }
};

typedef JsonDaikinEnaRes  JsonDaikinEnaRes_t;
typedef JsonDaikinEnaRes* JsonDaikinEnaRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonDaikinEnaRes::CreateJsonCommand(
    int_t devid,
    u8_t byValue
) {
    JsonCommand_p pJsonCommand = new JsonCommand("daikin", "enares");
    Json::Value jsonValue;
    jsonValue["devid"] = std::to_string(devid);
    jsonValue["ret"] = std::to_string(byValue);

    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINENARES_HPP_ */
