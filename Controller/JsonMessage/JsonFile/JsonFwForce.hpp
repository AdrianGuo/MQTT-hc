/*
 * JsonFwForce.hpp
 *
 *  Created on: Feb 23, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWFORCE_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWFORCE_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFwForce : public JsonMessageBase {
public:
    typedef struct {
        u8_t       	 type;
        String       version;
    } Firmware_t, *FileRes_p;

private:
    Firmware_t m_Firmware;
    bool_t ParseJsonValue(Json::Value& jsonValue);

public:
    JsonFwForce() {}
    virtual ~JsonFwForce() {}

    static String GetStrCmd() { return "fw=force"; }
    virtual void_t Refresh() { m_Firmware = {}; }

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    Firmware_t Firmware() const { return m_Firmware; }
};

typedef JsonFwForce  JsonFwForce_t;
typedef JsonFwForce* JsonFwForce_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFwForce::ParseJsonCommand(
    JsonCommand_p pJsonCommand
) {
    return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func   ParseJsonValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFwForce::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("type")||
    		!jsonValue.isMember("version")) { return FALSE; }
    m_Firmware.type = atoi(jsonValue["type"].asCString());
    m_Firmware.version = String(jsonValue["version"].asCString());

    return TRUE;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWFORCE_HPP_ */
