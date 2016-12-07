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

#ifndef CONTROLLER_JSONMESSAGE_JSONRULEACTV_HPP_
#define CONTROLLER_JSONMESSAGE_JSONRULEACTV_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRuleActv {
public:
    JsonRuleActv() {}
    virtual ~JsonRuleActv() {}
    static String GetStrCmd() { return "rule=actv"; }
};

typedef JsonRuleActv  JsonRuleAct_t;
typedef JsonRuleActv* JsonRuleAct_p;

#endif /* CONTROLLER_JSONMESSAGE_JSONRULEACTV_HPP_ */
