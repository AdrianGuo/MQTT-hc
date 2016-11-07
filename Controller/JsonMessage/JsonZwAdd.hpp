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

#ifndef CONTROLLER_JSONMESSAGE_JSONZWADD_HPP_
#define CONTROLLER_JSONMESSAGE_JSONZWADD_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwAdd {
private:
    int_t m_iAct;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonZwAdd() : m_iAct(-1) {}
    virtual ~JsonZwAdd() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "zw=add"; }
    int_t Act() const { return m_iAct; }
};

typedef JsonZwAdd  JsonZwAdd_t;
typedef JsonZwAdd* JsonZwAdd_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonZwAdd::ParseJsonCommand(
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
JsonZwAdd::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("act")) { return FALSE; }
    m_iAct = atoi(jsonValue["act"].asCString());
    return TRUE;
}


#endif /* CONTROLLER_JSONMESSAGE_JSONZWADD_HPP_ */
