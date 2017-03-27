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

#ifndef JSON_RULE_DELRES_HPP_
#define JSON_RULE_DELRES_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRuleDelRes {
public:
    JsonRuleDelRes() {}
    virtual ~JsonRuleDelRes() {}
    static String GetStrCmd() { return "rule=delres"; }
};

typedef JsonRuleDelRes  JsonRuleDelRes_t;
typedef JsonRuleDelRes* JsonRuleDelRes_p;

#endif /* JSON_RULE_DELRES_HPP_ */
