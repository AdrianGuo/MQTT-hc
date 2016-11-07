/*
 * JsonRGBEnableRes.hpp
 *
 *  Created on: Nov 7, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONRGBENABLERES_HPP_
#define CONTROLLER_JSONMESSAGE_JSONRGBENABLERES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRGBEnableRes {
public:
    JsonRGBEnableRes() {}
    virtual ~JsonRGBEnableRes() {}
    JsonCommand_p CreateJsonCommand(u8_t byValue);
    static String GetStrCmd() { return "rgb=enableres"; }
};

typedef JsonRGBEnableRes  JsonRGBEnableRes_t;
typedef JsonRGBEnableRes* JsonRGBEnableRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonRGBEnableRes::CreateJsonCommand(
    u8_t byValue
) {
    JsonCommand_p pJsonCommand = new JsonCommand("rgb", "enableres");
    Json::Value jsonValue;
    jsonValue["ret"] = std::to_string(byValue);

    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}


#endif /* CONTROLLER_JSONMESSAGE_JSONRGBENABLERES_HPP_ */
