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

#ifndef CONTROLLER_JSONMESSAGE_JSONRULEADD_HPP_
#define CONTROLLER_JSONMESSAGE_JSONRULEADD_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRuleAdd {
public:
    JsonRuleAdd() {}
    virtual ~JsonRuleAdd() {}
    static String GetStrCmd() { return "rule=add"; }
};

typedef JsonRuleAdd  JsonRuleAdd_t;
typedef JsonRuleAdd* JsonRuleAdd_p;

#endif /* CONTROLLER_JSONMESSAGE_JSONRULEADD_HPP_ */
