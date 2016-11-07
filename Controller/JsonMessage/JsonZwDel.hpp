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

#ifndef JSON_ZWDEL_HPP_
#define JSON_ZWDEL_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwDel {
private:
    int_t m_iAct;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonZwDel() : m_iAct(-1) {}
    virtual ~JsonZwDel() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);

    static String GetStrCmd() { return "zw=del"; }
    int_t Act() const { return m_iAct; }
};

typedef JsonZwDel  JsonZwDel_t;
typedef JsonZwDel* JsonZwDel_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonZwDel::ParseJsonCommand(
    JsonCommand_p pJsonComand
) {
    return ParseJsonValue(pJsonComand->GetJsonOjbect());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonZwDel::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("act")) { return FALSE; }
    m_iAct = atoi(jsonValue["act"].asCString());
    return TRUE;
}

#endif /* !JSON_ZWDEL_HPP_ */
