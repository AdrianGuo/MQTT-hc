/*
 * JsonZbRestartRes.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBRESTARTRES_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBRESTARTRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZbRestartRes {
public:
    JsonZbRestartRes() {}
    virtual ~JsonZbRestartRes() {}
    JsonCommand_p CreateJsonCommand(u8_t byRet);
    static String GetStrCmd() { return "zb=restartres"; }
};

typedef JsonZbRestartRes  JsonZbRestartRes_t;
typedef JsonZbRestartRes* JsonZbRestartRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZbRestartRes::CreateJsonCommand(
    u8_t byRet
) {
    JsonCommand_p pJsonCommand = new JsonCommand("zb", "restartres");

    Json::Value jsonValue;
    jsonValue["ret"] = std::to_string(byRet);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBRESTARTRES_HPP_ */
