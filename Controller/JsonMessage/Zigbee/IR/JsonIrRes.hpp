/*
 * JsonIrRes.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRRES_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonIrRes {
public:
    JsonIrRes() {}
    virtual ~JsonIrRes() {}
    JsonCommand_p CreateJsonCommand(int_t devid, int_t ord, u8_t ret, int_t irid = 0);
    static String GetStrCmd() { return "ir=res"; }
};

typedef JsonIrRes  JsonIrRes_t;
typedef JsonIrRes* JsonIrRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonIrRes::CreateJsonCommand(
    int_t devid,
    int_t ord,
    u8_t ret,
    int_t irid
) {
    JsonCommand_p pJsonCommand = new JsonCommand("ir", "res");

    Json::Value jsonValue;
    jsonValue["devid"] = std::to_string(devid);
    jsonValue["ord"] = std::to_string(ord);
    jsonValue["ret"] = std::to_string(ret);
    jsonValue["irid"] = std::to_string(irid);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRRES_HPP_ */
