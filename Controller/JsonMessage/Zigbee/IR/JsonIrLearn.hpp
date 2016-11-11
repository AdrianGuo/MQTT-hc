/*
 * JsonIrLearn.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_JSONIRLEARN_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_JSONIRLEARN_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

typedef struct {
    int_t devid;
    int_t ord;
    u8_t act;
}IrLearn_t;

class JsonIrLearn {
private:
    IrLearn_t m_IrLearn;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonIrLearn() {}
    virtual ~JsonIrLearn() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "ir=learn"; }
    IrLearn_t Return() const { return m_IrLearn; }
};

typedef JsonIrLearn  JsonIrLearn_t;
typedef JsonIrLearn* JsonIrLearn_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonIrLearn::ParseJsonCommand(
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
JsonIrLearn::ParseJsonValue(
    Json::Value& jsonValue
) {
    m_IrLearn = {};
    if (!jsonValue.isMember("devid") || !jsonValue.isMember("ord") || !jsonValue.isMember("act")) { return FALSE; }
    u8_t temp = atoi(jsonValue["act"].asCString());
    if ((temp < 0) || (temp > 1)) { return FALSE; }
    m_IrLearn.act = temp;
    m_IrLearn.devid = atoi(jsonValue["devid"].asCString());
    m_IrLearn.ord = atoi(jsonValue["ord"].asCString());
    return TRUE;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_JSONIRLEARN_HPP_ */
