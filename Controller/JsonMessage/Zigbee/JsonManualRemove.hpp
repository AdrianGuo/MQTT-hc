/*
 * JsonManualRemove.hpp
 *
 *  Created on: Nov 12, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_JSONMANUALREMOVE_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_JSONMANUALREMOVE_HPP_
#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonManualRemove {
private:
    int_t m_Nwk;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonManualRemove() : m_Nwk(0) {}
    virtual ~JsonManualRemove() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "opt=remove"; }
    int_t Return() const { return m_Nwk; }
};

typedef JsonManualRemove  JsonManualRemove_t;
typedef JsonManualRemove* JsonManualRemove_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonManualRemove::ParseJsonCommand(
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
JsonManualRemove::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("nwk")) { return FALSE; }
    m_Nwk = atoi(jsonValue["nwk"].asCString());
    return TRUE;
}
#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_JSONMANUALREMOVE_HPP_ */
