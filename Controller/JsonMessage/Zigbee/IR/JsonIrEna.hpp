/*
 * JsonIrEna.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRENA_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRENA_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

typedef struct {
    int_t irid;
    bool_t act;
}IrEna_t;

class JsonIrEna {
private:
    IrEna_t m_IrEna;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonIrEna() : m_IrEna({ 0, -1}) {}
    virtual ~JsonIrEna() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "ir=ena"; }
    IrEna_t Return() const { return m_IrEna; }
};

typedef JsonIrEna  JsonIrEna_t;
typedef JsonIrEna* JsonIrEna_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonIrEna::ParseJsonCommand(
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
JsonIrEna::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("irid") || !jsonValue.isMember("act")) { return FALSE; }
    int_t temp = atoi(jsonValue["act"].asCString());
    if ((temp < 0) || (temp > 1)) { return FALSE; }
    m_IrEna.act = temp;
    m_IrEna.irid = atoi(jsonValue["irid"].asCString());
    return TRUE;
}

#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIRENA_HPP_ */
