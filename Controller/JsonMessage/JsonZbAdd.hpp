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

#ifndef CONTROLLER_JSONMESSAGE_JSONZBADD_HPP_
#define CONTROLLER_JSONMESSAGE_JSONZBADD_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZbAdd {
private:
    int_t m_iAct;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonZbAdd() : m_iAct(-1) {}
    virtual ~JsonZbAdd() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "zb=add"; }
    int_t Act() const { return m_iAct; }
};

typedef JsonZbAdd  JsonZbAdd_t;
typedef JsonZbAdd* JsonZbAdd_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonZbAdd::ParseJsonCommand(
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
JsonZbAdd::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("act")) { return FALSE; }
    m_iAct = atoi(jsonValue["act"].asCString());
    return TRUE;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONZBADD_HPP_ */
