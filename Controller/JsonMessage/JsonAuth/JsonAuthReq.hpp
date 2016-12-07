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

#ifndef JSON_AUTH_REQ_HPP_
#define JSON_AUTH_REQ_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonAuthReq : public JsonMessageBase {
public:
    JsonAuthReq() {}
    virtual ~JsonAuthReq() {}

    static String GetStrCmd() { return "auth=req"; }
    virtual void_t Refresh() {}
    virtual JsonCommand_p CreateJsonCommand(u16_t wMacID);
    virtual JsonCommand_p CreateJsonCommand(String strMacID);
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonAuthReq::CreateJsonCommand(
    u16_t wMacID
) {
    Json::Value root;
    JsonCommand_p pJsonCommand = new JsonCommand("auth=req");

    root["mac"] = std::to_string(wMacID);
    pJsonCommand->SetJsonObject(root);

    return pJsonCommand;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonAuthReq::CreateJsonCommand(
    String strMacID
) {
    Json::Value root;
    JsonCommand_p pJsonCommand = new JsonCommand("auth=req");

    root["mac"] = strMacID.c_str();
    pJsonCommand->SetJsonObject(root);

    return pJsonCommand;
}

typedef JsonAuthReq  JsonAuthReq_t;
typedef JsonAuthReq* JsonAuthReq_p;

#endif /* !JSON_AUTH_REQ_HPP_ */
