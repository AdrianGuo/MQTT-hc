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

class JsonIrSet {
private:
    int_t m_iIrID;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonIrSet() : m_iIrID(0) {}
    virtual ~JsonIrSet() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "ir=set"; }
    int_t Set() const { return m_iIrID; }
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
    if (!jsonValue.isMember("irid")) { return FALSE; }
    m_iIrID = atoi(jsonValue["irid"].asCString());
    return TRUE;
}

#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRSET_HPP_ */
