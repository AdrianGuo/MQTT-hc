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

#ifndef CONTROLLER_JSONMESSAGE_JSONZBGET_HPP_
#define CONTROLLER_JSONMESSAGE_JSONZBGET_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "Vector.hpp"

typedef struct {
    int_t devid;
    int_t ord;
}ZbGet_t;

class JsonZbGet {
private:
    Vector<ZbGet_t> m_vZbGet;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonZbGet() {}
    virtual ~JsonZbGet() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "dev=get"; }
    Vector<ZbGet_t> Return() const { return m_vZbGet; }
};

typedef JsonZbGet  JsonZbGet_t;
typedef JsonZbGet* JsonZbGet_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonZbGet::ParseJsonCommand(
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
JsonZbGet::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (jsonValue.isMember("dev")) {
        const Json::Value& devs = jsonValue["dev"];
        for(Json::ValueConstIterator it = devs.begin(); it != devs.end(); ++it) {
            if (!(*it).isMember("devid") ||
                !(*it).isMember("ord")) { continue; }
            ZbGet_t device;
            device.devid = std::stoi((*it)["devid"].asCString());
            device.ord = std::stoi((*it)["ord"].asCString());
            m_vZbGet.push_back(device);
        }
    } else if (!jsonValue.isMember("dev")){
        if (!jsonValue.isMember("devid") ||
            !jsonValue.isMember("ord")) { return FALSE; }
        ZbGet_t device;
        device.devid = std::stoi(jsonValue["devid"].asCString());
        device.ord = std::stoi(jsonValue["ord"].asCString());
        m_vZbGet.push_back(device);
    }
    return TRUE;
}


#endif /* CONTROLLER_JSONMESSAGE_JSONZBGET_HPP_ */
