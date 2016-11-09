/*
 * JsonZbSet.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBSET_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBSET_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "Vector.hpp"

typedef struct {
    int_t devid;
    int_t order;
    int_t value;
}ZbSet_t;

class JsonZbSet {
private:
    Vector<ZbSet_t> m_vZbSet;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonZbSet() {}
    virtual ~JsonZbSet() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "zb=set"; }
    Vector<ZbSet_t> Return() const { return m_vZbSet; }
};

typedef JsonZbSet  JsonZbSet_t;
typedef JsonZbSet* JsonZbSet_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonZbSet::ParseJsonCommand(
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
JsonZbSet::ParseJsonValue(
    Json::Value& jsonValue
) {
    m_vZbSet.clear();

    if (jsonValue.isMember("dev")) {
        const Json::Value& devs = jsonValue["dev"];
        for(Json::ValueConstIterator it = devs.begin(); it != devs.end(); ++it) {
            if (!(*it).isMember("devid") ||
                !(*it).isMember("ord")   ||
                !(*it).isMember("val")) { continue; }
            ZbSet_t device;
            device.devid = std::stoi((*it)["devid"].asCString());
            device.order = std::stoi((*it)["ord"].asCString());
            device.value = std::stoi((*it)["val"].asCString());
            m_vZbSet.push_back(device);
        }
    } else if (!jsonValue.isMember("dev")) {
        if (!jsonValue.isMember("devid") ||
            !jsonValue.isMember("ord")   ||
            !jsonValue.isMember("val")) { return FALSE; }
        ZbSet_t device;
        device.devid = std::stoi(jsonValue["devid"].asCString());
        device.order = std::stoi(jsonValue["ord"].asCString());
        device.value = std::stoi(jsonValue["val"].asCString());
        m_vZbSet.push_back(device);
    }

    return TRUE;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_JSONZBSET_HPP_ */
