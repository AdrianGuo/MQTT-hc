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

#ifndef JSON_ZWRESTART_HPP_
#define JSON_ZWRESTART_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwRestart {
    virtual bool_t ParseJsonValue(Json::Value& jsonValue) { return FALSE; }
public:
    JsonZwRestart() {}
    virtual ~JsonZwRestart() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand) { return FALSE; }
    static String GetStrCmd() { return "rz=restart"; }
};

typedef JsonZwRestart  JsonZwRestart_t;
typedef JsonZwRestart* JsonZwRestart_p;

#endif /* !JSON_ZWRESTART_HPP_ */
