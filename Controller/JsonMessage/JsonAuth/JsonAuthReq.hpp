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

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonAuthReq : public JsonMessageBase {
public:
    typedef struct {
        String       mac;
        u8_t         type;
    } Device_t, *Device_p;
private:
    Device_t m_Client;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonAuthReq() {}
    virtual ~JsonAuthReq() {}

    static String GetStrCmd() { return "auth=req"; }
    virtual void_t Refresh() { m_Client = {}; }
    virtual JsonCommand_p CreateJsonCommand(u16_t wMacID);
    virtual JsonCommand_p CreateJsonCommand(String strMacID);
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    Device_t Dev() const { return m_Client; }
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

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonAuthReq::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("mac") || !jsonValue.isMember("type")) { return FALSE; }
    m_Client.mac    = String(jsonValue["mac"].asCString());
    m_Client.type   = atoi(jsonValue["type"].asCString());
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonAuthReq::ParseJsonCommand(
    JsonCommand_p pJsonCommand
) {
    return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

typedef JsonAuthReq  JsonAuthReq_t;
typedef JsonAuthReq* JsonAuthReq_p;

#endif /* !JSON_AUTH_REQ_HPP_ */
