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

#ifndef JSON_AUTH_RES_HPP_
#define JSON_AUTH_RES_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonAuthRes : public JsonMessageBase {
private:
    i8_t  m_ibyRet;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonAuthRes() : m_ibyRet (-1) {}
    virtual ~JsonAuthRes() {}

    static String GetStrCmd() { return "auth=res"; }
    virtual void_t Refresh() { m_ibyRet = -1; }
    JsonCommand_p CreateJsonCommand(u8_t ret, u16_t devid);
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    i32_t Ret() const { return m_ibyRet; }
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonAuthRes::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("ret")) { return FALSE; }
    m_ibyRet = std::stoi(jsonValue["ret"].asCString());
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonAuthRes::ParseJsonCommand(
    JsonCommand_p pJsonCommand
) {
    return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonAuthRes::CreateJsonCommand(
    u8_t ret,
    u16_t devid
) {
    JsonCommand_p pJsonCommand = new JsonCommand("auth=res");
    Json::Value jsonValue;

    jsonValue["ret"] = std::to_string(ret);
    jsonValue["devid"] = std::to_string(devid);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}
#endif /* !JSON_AUTH_RES_HPP_ */
