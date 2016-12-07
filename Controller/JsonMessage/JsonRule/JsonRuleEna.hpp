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

#ifndef CONTROLLER_JSONMESSAGE_JSONRULEENA_HPP_
#define CONTROLLER_JSONMESSAGE_JSONRULEENA_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRuleEna {
public:
    JsonRuleEna() {}
    virtual ~JsonRuleEna() {}
    static String GetStrCmd() { return "rule=ena"; }
};

typedef JsonRuleEna  JsonRuleEna_t;
typedef JsonRuleEna* JsonRuleEna_p;

#endif /* CONTROLLER_JSONMESSAGE_JSONRULEENA_HPP_ */
