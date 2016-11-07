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
 * Last Changed:     Date: 2016-10-31 19:40:00 (Mon, 31 Oct 2016)
 *
 ******************************************************************************/

#ifndef JSON_ZWADDRES_HPP_
#define JSON_ZWADDRES_HPP_


#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwAddRes {
public:
    JsonZwAddRes() {}
    virtual ~JsonZwAddRes() {}
    JsonCommand_p CreateJsonCommand(u8_t byRet);
    static String GetStrCmd() { return "zw=addres"; }
};

typedef JsonZwAddRes  JsonZwAddRes_t;
typedef JsonZwAddRes* JsonZwAddRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZwAddRes::CreateJsonCommand(
    u8_t byRet
) {
    JsonCommand_p pJsonCommand = new JsonCommand("zw", "addres");

    Json::Value jsonValue;
    jsonValue["ret"] = std::to_string(byRet);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}


#endif /* !JSON_ZWADDRES_HPP_ */
