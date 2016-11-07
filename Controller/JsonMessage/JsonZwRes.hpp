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
 * Last Changed:     Date: 2016-10-31 16:00:00 (Mon, 31 Oct 2016)
 *
 ******************************************************************************/

#ifndef JSON_ZWRES_HPP_
#define JSON_ZWRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwRes {
public:
    JsonZwRes() {}
    virtual ~JsonZwRes() {}
    JsonCommand_p CreateJsonCommand(u8_t byRet);
    static String GetStrCmd() { return "zw=res"; }
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZwRes::CreateJsonCommand(
    u8_t byRet
) {
    JsonCommand_p pJsonCommand = new JsonCommand("zw", "res");

    Json::Value jsonValue;
    jsonValue["ret"] = std::to_string(byRet);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

typedef JsonZwRes  JsonZwRes_t;
typedef JsonZwRes* JsonZwRes_p;

#endif /* !JSON_ZWRES_HPP_ */
