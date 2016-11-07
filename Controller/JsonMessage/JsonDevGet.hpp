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

#ifndef JSON_DEV_GET_HPP_
#define JSON_DEV_GET_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevGet {
public:
    JsonDevGet() {}
    virtual ~JsonDevGet() {}

    virtual bool_t ParseJsonValue(Json::Value& jsonValue) { return FALSE;}
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand) { return FALSE; }

    static const String GetStrCmd() { return "dev=get"; }
};

typedef JsonDevGet  JsonDevGet_t;
typedef JsonDevGet* JsonDevGet_p;


#endif /* !JSON_DEV_GET_HPP_ */
