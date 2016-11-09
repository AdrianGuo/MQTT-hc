/*
 * JsonDaikinEna.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINENA_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINENA_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

typedef struct {
    int_t devid;
    bool_t act;
} DaikinEna_t;

class JsonDaikinEna {
private:
    DaikinEna_t m_DaikinEna;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonDaikinEna() : m_DaikinEna({0, 0}) {}
    virtual ~JsonDaikinEna() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "daikin=ena"; }
    DaikinEna_t Return() const { return m_DaikinEna; }
};

typedef JsonDaikinEna  JsonDaikinEna_t;
typedef JsonDaikinEna* JsonDaikinEna_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDaikinEna::ParseJsonCommand(
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
JsonDaikinEna::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("devid") || !jsonValue.isMember("act")) { return FALSE; }
    u8_t temp = atoi(jsonValue["act"].asCString());
    if((temp < 0) && (temp > 1)) { return FALSE; }
    m_DaikinEna.act = temp;
    m_DaikinEna.devid = atoi(jsonValue["devid"].asCString());
    return TRUE;
}

#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_DAIKIN_JSONDAIKINENA_HPP_ */
