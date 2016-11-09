/*
 * JsonZbResetRes.hpp
 *
 *  Created on: Nov 6, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONZBRESETRES_HPP_
#define CONTROLLER_JSONMESSAGE_JSONZBRESETRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZbResetRes {
public:
    JsonZbResetRes() {}
    virtual ~JsonZbResetRes() {}
    JsonCommand_p CreateJsonCommand(u8_t byRet);
    static String GetStrCmd() { return "zb=resetres"; }
};

typedef JsonZbResetRes  JsonZbResetRes_t;
typedef JsonZbResetRes* JsonZbResetRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZbResetRes::CreateJsonCommand(
    u8_t byRet
) {
    JsonCommand_p pJsonCommand = new JsonCommand("zb", "resetres");

    Json::Value jsonValue;
    jsonValue["ret"] = std::to_string(byRet);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONZBRESETRES_HPP_ */
