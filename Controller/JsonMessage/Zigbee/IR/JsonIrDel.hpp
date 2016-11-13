/*
 * JsonIrDel.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRDEL_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRDEL_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

typedef struct {
    int_t devid;
    int_t ord;
    int_t irid;
    bool_t act;
}IrDel_t;

class JsonIrDel {
private:
    IrDel_t m_IrDel;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonIrDel() : m_IrDel({ 0, -1}) {}
    virtual ~JsonIrDel() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "ir=del"; }
    IrDel_t Return() const { return m_IrDel; }
};

typedef JsonIrDel  JsonIrDel_t;
typedef JsonIrDel* JsonIrDel_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonIrDel::ParseJsonCommand(
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
JsonIrDel::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("devid") ||
            !jsonValue.isMember("ord") ||
            !jsonValue.isMember("irid") ||
            !jsonValue.isMember("act")) { return FALSE; }
    int_t temp = atoi(jsonValue["act"].asCString());
    if ((temp < 0) || (temp > 1)) { return FALSE; }
    m_IrDel.act = temp;
    m_IrDel.devid = atoi(jsonValue["devid"].asCString());
    m_IrDel.ord = atoi(jsonValue["ord"].asCString());
    m_IrDel.irid = atoi(jsonValue["irid"].asCString());
    return TRUE;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRDEL_HPP_ */
