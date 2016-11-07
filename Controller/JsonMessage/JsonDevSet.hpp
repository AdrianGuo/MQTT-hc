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

#ifndef JSON_DEV_SET_HPP_
#define JSON_DEV_SET_HPP_


#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevSet {
public:
    JsonDevSet() {}
    virtual ~JsonDevSet() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);

    static String GetStrCmd() { return "dev=set"; }
};

typedef JsonDevSet  JsonDevSet_t;
typedef JsonDevSet* JsonDevSet_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDevSet::ParseJsonCommand(
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
JsonDevSet::ParseJsonValue(
    Json::Value& jsonValue
) {

    return TRUE;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONDEVSET_HPP_ */
