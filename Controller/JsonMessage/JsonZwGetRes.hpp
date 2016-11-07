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
 * Last Changed:     Date: 2016-10-31 19:40:00 (Mon, 31 Oct 2016)
 *
 ******************************************************************************/

#ifndef JSON_ZWGETRES_HPP_
#define JSON_ZWGETRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwGetRes  {
private:
    virtual bool_t ParseJsonValue(Json::Value& jsonValue) { return FALSE; }
public:
    JsonZwGetRes() {}
    virtual ~JsonZwGetRes() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand) { return FALSE; }

    static String GetStrCmd() { return "zw=getres"; }
};

typedef JsonZwGetRes  JsonZwGetRes_t;
typedef JsonZwGetRes* JsonZwGetRes_p;

#endif /* !JSON_ZWGETRES_HPP_ */
