/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: JsonDevAdd.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 7 Nov 2016 10:04:22
 *
 ******************************************************************************/
#ifndef JSON_DEV_ADD_HPP_
#define JSON_DEV_ADD_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevAdd : public JsonMessageBase {
private:
    i8_t m_ibAct;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonDevAdd() : m_ibAct (-1) {}
    virtual ~JsonDevAdd() {}

    static String GetStrCmd() { return "dev=add"; }
    virtual void_t Refresh() { m_ibAct = -1; }

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    JsonCommand_p CreateJsonCommand(i8_t ibAct);
    i8_t Act() const { return m_ibAct; }
};

typedef JsonDevAdd  JsonDevAdd_t;
typedef JsonDevAdd* JsonDevAdd_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDevAdd::ParseJsonCommand(
    JsonCommand_p pJsonCommand
) {
    return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func   ParseJsonValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDevAdd::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("act")) { return FALSE; }
    m_ibAct = std::stoi(jsonValue["act"].asCString());
    return TRUE;
}

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonDevAdd::CreateJsonCommand(
    i8_t ibAct
) {
    JsonCommand_p pJsonCommand = new JsonCommand("dev=add");
    Json::Value jsonValue;

    jsonValue["act"] = std::to_string(ibAct);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* !JSON_DEV_ADD_HPP_ */
