/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-25 14:00:00 (Tue, 25 Oct 2016)
 *
 ******************************************************************************/

#ifndef JSON_ZWRESETRES_HPP_
#define JSON_ZWRESETRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwResetRes {
public:
    JsonZwResetRes() {}
    virtual ~JsonZwResetRes() {}
    JsonCommand_p CreateJsonCommand(u8_t byRet);
    static String GetStrCmd() { return "zw=resetres"; }
};

typedef JsonZwResetRes  JsonZwResetRes_t;
typedef JsonZwResetRes* JsonZwResetRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZwResetRes::CreateJsonCommand(
    u8_t byRet
) {
    JsonCommand_p pJsonCommand = new JsonCommand("zw", "resetres");

    Json::Value jsonValue;
    jsonValue["ret"] = std::to_string(byRet);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* !JSON_ZWRESETRES_HPP_ */
