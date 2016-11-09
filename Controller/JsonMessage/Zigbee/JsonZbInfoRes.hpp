/*
 * JsonZbInfoRes.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBINFORES_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBINFORES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZbInfoRes {
public:
    JsonZbInfoRes() {}
    virtual ~JsonZbInfoRes() {}
    JsonCommand_p CreateJsonCommand(u8_t byRet);
    static String GetStrCmd() { return "zb=infores"; }
};

typedef JsonZbInfoRes  JsonZbInfoRes_t;
typedef JsonZbInfoRes* JsonZbInfoRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZbInfoRes::CreateJsonCommand(
    u8_t byRet
) {
    JsonCommand_p pJsonCommand = new JsonCommand("zb", "infores");

    Json::Value jsonValue;
    jsonValue["ret"] = std::to_string(byRet);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBINFORES_HPP_ */
