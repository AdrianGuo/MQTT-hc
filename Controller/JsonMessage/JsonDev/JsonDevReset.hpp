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
 * Last Changed:     Date: 2016-05-16 11:45:00 (Tue, 16 May 2016)
 *
 ******************************************************************************/

#ifndef JSON_DEV_RESET_HPP_
#define JSON_DEV_RESET_HPP_

#include "Typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevReset : public JsonMessageBase {
private:
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonDevReset() {}
    virtual ~JsonDevReset() {}

    static String GetStrCmd() { return "dev=reset"; }
    virtual void_t Refresh() { }

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    JsonCommand_p CreateJsonCommand();
};

typedef JsonDevReset  JsonDevReset_t;
typedef JsonDevReset* JsonDevReset_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDevReset::ParseJsonCommand(
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
JsonDevReset::ParseJsonValue(
    Json::Value& jsonValue
) {
    return TRUE;
}

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonDevReset::CreateJsonCommand() {
    Json::Reader reader;
    Json::Value root;
    reader.parse("{}", root, false);
    JsonCommand_p pJsonCommand = new JsonCommand("dev=reset");
    pJsonCommand->SetJsonObject(root);

    return pJsonCommand;
}

#endif /* !JSON_DEV_RESET_HPP_ */
