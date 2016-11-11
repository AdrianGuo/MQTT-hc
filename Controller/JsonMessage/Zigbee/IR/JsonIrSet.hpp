/*
 * JsonIrSet.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRSET_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRSET_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

typedef struct {
    int_t devid;
    int_t ord;
    int_t irid;
}IrSet_t;

class JsonIrSet {
private:
    IrSet_t m_IrSet;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonIrSet() {}
    virtual ~JsonIrSet() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "ir=set"; }
    IrSet_t Return() const { return m_IrSet; }
};

typedef JsonIrSet  JsonIrSet_t;
typedef JsonIrSet* JsonIrSet_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonIrSet::ParseJsonCommand(
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
JsonIrSet::ParseJsonValue(
    Json::Value& jsonValue
) {
    m_IrSet = {};
    if (!jsonValue.isMember("devid") ||
            !jsonValue.isMember("ord") ||
            !jsonValue.isMember("irid")) { return FALSE; }
    m_IrSet.devid = atoi(jsonValue["devid"].asCString());
    m_IrSet.ord = atoi(jsonValue["ord"].asCString());
    m_IrSet.irid = atoi(jsonValue["irid"].asCString());
    return TRUE;
}

#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRSET_HPP_ */
