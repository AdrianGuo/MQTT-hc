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

#ifndef JSON_DEV_RESETRES_HPP_
#define JSON_DEV_RESETRES_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevResetRes : public JsonMessageBase {
public:
    typedef struct {
        u8_t ret;
        u8_t netwk;
    } Device_t, *Device_p;

    JsonDevResetRes() {}
    virtual ~JsonDevResetRes() {}

    static String GetStrCmd() { return "dev=resetres"; }
    virtual void_t Refresh() { }
    JsonCommand_p CreateJsonCommand(Device_t dev);
};

typedef JsonDevResetRes  JsonDevResetRes_t;
typedef JsonDevResetRes* JsonDevResetRes_p;

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonDevResetRes::CreateJsonCommand(
    Device_t dev
) {
    JsonCommand_p pJsonCommand = new JsonCommand("dev=resetres");
    Json::Value jsonValue;

    jsonValue["net"] = std::to_string(dev.netwk);
    jsonValue["ret"] = std::to_string(dev.ret);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* !JSON_DEV_RESETRES_HPP_ */
