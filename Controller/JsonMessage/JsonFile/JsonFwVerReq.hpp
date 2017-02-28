/*
 * JsonFwInfoReq.hpp
 *
 *  Created on: Feb 22, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWVERREQ_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWVERREQ_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFwVerReq : public JsonMessageBase {
private:
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonFwVerReq() {}
    virtual ~JsonFwVerReq() {}

    static String GetStrCmd() { return "fw=verreq"; }
    virtual void_t Refresh() {}

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    JsonCommand_p CreateJsonCommand(String strType);
};

typedef JsonFwVerReq  JsonFwInfoReq_t;
typedef JsonFwVerReq* JsonFwInfoReq_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFwVerReq::ParseJsonCommand(
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
JsonFwVerReq::ParseJsonValue(
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
JsonFwVerReq::CreateJsonCommand(
	String strType
) {
    JsonCommand_p pJsonCommand = new JsonCommand("fw=verreq");
    Json::Value jsonValue;
    jsonValue["type"] = std::string(strType.c_str());
    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWVERREQ_HPP_ */
