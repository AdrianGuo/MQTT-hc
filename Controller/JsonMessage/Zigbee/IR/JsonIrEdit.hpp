/*
 * JsonIrEdit.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIREDIT_HPP_
#define CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIREDIT_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

typedef struct {
    int_t devid;
    int_t ord;
    int_t irid;
    String name;
}IrEdit_t;

class JsonIrEdit {
private:
    IrEdit_t m_IrEdit;
    virtual bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonIrEdit() {}
    virtual ~JsonIrEdit() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    static String GetStrCmd() { return "ir=edit"; }
    IrEdit_t Return() const { return m_IrEdit; }
};

typedef JsonIrEdit  JsonIrEdit_t;
typedef JsonIrEdit* JsonIrEdit_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonIrEdit::ParseJsonCommand(
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
JsonIrEdit::ParseJsonValue(
    Json::Value& jsonValue
) {
    m_IrEdit = {};
    if (!jsonValue.isMember("devid") ||
            !jsonValue.isMember("ord") ||
            !jsonValue.isMember("irid") ||
            !jsonValue.isMember("name")) { return FALSE; }
    m_IrEdit.devid = atoi(jsonValue["devid"].asCString());
    m_IrEdit.ord = atoi(jsonValue["ord"].asCString());
    m_IrEdit.irid = atoi(jsonValue["irid"].asCString());
    m_IrEdit.name = String(jsonValue["name"].asCString());
    return TRUE;
}


#endif /* CONTROLLER_JSONMESSAGE_ZIGBEE_IR_JSONIREDIT_HPP_ */
