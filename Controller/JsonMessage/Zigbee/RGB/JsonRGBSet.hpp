/*
 * JsonRGBSet.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_RGB_JSONRGBSET_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_RGB_JSONRGBSET_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

typedef struct {
    int_t devid;
    int_t red;
    int_t green;
    int_t blue;
    u8_t time;
} RGBSet_t;

class JsonRGBSet {
private:
    RGBSet_t m_RGBSet;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonRGBSet() : m_RGBSet({0, 0, 0, 0}) {}
    virtual ~JsonRGBSet() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "rgb=set"; }
    RGBSet_t Return() const { return m_RGBSet; }
};

typedef JsonRGBSet  JsonRGBSet_t;
typedef JsonRGBSet* JsonRGBSet_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRGBSet::ParseJsonCommand(
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
JsonRGBSet::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("devid") || !jsonValue.isMember("val")) { return FALSE; }

    std::string temp = jsonValue["val"].asString();
    int_t pos1stCol = temp.find(":");
    if(pos1stCol == std::string::npos) {return FALSE;}
    m_RGBSet.red = atoi(temp.substr(0, pos1stCol).c_str());

    temp = temp.substr(pos1stCol, temp.size() - pos1stCol);
    int_t pos2ndCol = temp.find(":");
    if(pos2ndCol == std::string::npos) {return FALSE;}
    m_RGBSet.green = atoi(temp.substr(0, pos2ndCol).c_str());

    temp = temp.substr(pos2ndCol, temp.size() - pos2ndCol);
    int_t pos3rdCol = temp.find(":");
    if(pos3rdCol == std::string::npos) {return FALSE;}
    m_RGBSet.blue = atoi(temp.substr(0, pos3rdCol).c_str());

    temp = temp.substr(pos3rdCol, temp.size() - pos3rdCol);
    int_t pos4thCol = temp.find(":");
    if(pos4thCol == std::string::npos) {return FALSE;}
    m_RGBSet.time = (u8_t) atoi(temp.substr(0, pos4thCol).c_str());

    m_RGBSet.devid = atoi(jsonValue["devid"].asCString());
    return TRUE;
}

#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_RGB_JSONRGBSET_HPP_ */
