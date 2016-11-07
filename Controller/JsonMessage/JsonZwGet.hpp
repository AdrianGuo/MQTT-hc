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

#ifndef CONTROLLER_JSONMESSAGE_JSONZWGET_HPP_
#define CONTROLLER_JSONMESSAGE_JSONZWGET_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwGet  {
private:
    virtual bool_t ParseJsonValue(Json::Value& jsonValue) { return FALSE; }
public:
    JsonZwGet() {}
    virtual ~JsonZwGet() {}

    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand) { return FALSE; }

    static String GetStrCmd() { return "zw=get"; }
};

typedef JsonZwGet  JsonZwGet_t;
typedef JsonZwGet* JsonZwGet_p;

#endif /* CONTROLLER_JSONMESSAGE_JSONZWGET_HPP_ */
