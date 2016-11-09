/*
 * JsonRGBGet.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_RGB_JSONRGBGET_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_RGB_JSONRGBGET_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRGBGet {
private:
    int_t m_iDevID;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonRGBGet() : m_iDevID(0) {}
    virtual ~JsonRGBGet() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "rgb=get"; }
    int_t Return() const { return m_iDevID; }
};

typedef JsonRGBGet  JsonRGBGet_t;
typedef JsonRGBGet* JsonRGBGet_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRGBGet::ParseJsonCommand(
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
JsonRGBGet::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("devid")) { return FALSE; }
    m_iDevID = atoi(jsonValue["devid"].asCString());
    return TRUE;
}
#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_RGB_JSONRGBGET_HPP_ */
