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
    JsonCommand_p CreateJsonCommand(int_t idwIrID, u8_t byRet);
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
    int_t idwIrID,
    u8_t byRet
) {
    JsonCommand_p pJsonCommand = new JsonCommand("ir", "res");

    Json::Value jsonValue;
    jsonValue["irid"] = std::to_string(idwIrID);
    jsonValue["ret"] = std::to_string(byRet);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRRES_HPP_ */
