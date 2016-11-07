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

class JsonAuthRes {
private:
    int_t  m_iRet;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonAuthRes();
    virtual ~JsonAuthRes() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    static String GetStrCmd() { return "auth=res"; }

    int_t Ret() const { return m_iRet; }
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline
JsonAuthRes::JsonAuthRes() : m_iRet (-1) {
}

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
    m_iRet = atoi(jsonValue["ret"].asCString());
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

#endif /* !JSON_AUTH_RES_HPP_ */
