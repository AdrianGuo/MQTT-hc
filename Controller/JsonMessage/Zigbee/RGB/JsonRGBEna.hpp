/*
 * JsonRGBEna.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_RGB_JSONRGBENA_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_RGB_JSONRGBENA_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

typedef struct {
    int_t devid;
    bool_t act;
} RGBEna_t;

class JsonRGBEna {
private:
    RGBEna_t m_RGBEna;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonRGBEna() : m_RGBEna({0, 0}) {}
    virtual ~JsonRGBEna() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "rgb=ena"; }
    RGBEna_t Return() const { return m_RGBEna; }
};

typedef JsonRGBEna  JsonRGBEna_t;
typedef JsonRGBEna* JsonRGBEna_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRGBEna::ParseJsonCommand(
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
JsonRGBEna::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("devid") || !jsonValue.isMember("act")) { return FALSE; }
    u8_t temp = atoi(jsonValue["act"].asCString());
    if((temp < 0) && (temp > 1)) { return FALSE; }
    m_RGBEna.act = temp;
    m_RGBEna.devid = atoi(jsonValue["devid"].asCString());
    return TRUE;
}
#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_RGB_JSONRGBENA_HPP_ */
