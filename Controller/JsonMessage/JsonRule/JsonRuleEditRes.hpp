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

#ifndef CONTROLLER_JSONMESSAGE_JSONRULEEDITRES_HPP_
#define CONTROLLER_JSONMESSAGE_JSONRULEEDITRES_HPP_


#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRuleEditRes {
public:
    JsonRuleEditRes() {}
    virtual ~JsonRuleEditRes() {}
    static String GetStrCmd() { return "rule=editres"; }
};

typedef JsonRuleEditRes  JsonRuleEditRes_t;
typedef JsonRuleEditRes* JsonRuleEditRes_p;

#endif /* CONTROLLER_JSONMESSAGE_JSONRULEEDITRES_HPP_ */