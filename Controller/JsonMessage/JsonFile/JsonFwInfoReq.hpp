/*
 * JsonFwInfoReq.hpp
 *
 *  Created on: Feb 22, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWINFOREQ_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWINFOREQ_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFwInfoReq : public JsonMessageBase {
private:
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonFwInfoReq() {}
    virtual ~JsonFwInfoReq() {}

    static String GetStrCmd() { return "fw=inforeq"; }
    virtual void_t Refresh() {}

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    JsonCommand_p CreateJsonCommand();
};

typedef JsonFwInfoReq  JsonFwInfoReq_t;
typedef JsonFwInfoReq* JsonFwInfoReq_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFwInfoReq::ParseJsonCommand(
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
JsonFwInfoReq::ParseJsonValue(
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
JsonFwInfoReq::CreateJsonCommand(
) {
    return new JsonCommand("fw=inforeq", "{}");
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWINFOREQ_HPP_ */
