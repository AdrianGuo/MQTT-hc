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

#ifndef JSON_FIRM_UPDCTRL_HPP_
#define JSON_FIRM_UPDCTRL_HPP_


#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFirmUpdCtrl {
public:
    JsonFirmUpdCtrl() {}
    virtual ~JsonFirmUpdCtrl() {}
    static String GetStrCmd() { return "firm=updctrl"; }
};

typedef JsonFirmUpdCtrl  JsonFirmUpdCtrl_t;
typedef JsonFirmUpdCtrl* JsonFirmUpdCtrl_p;


#endif /* !JSON_FIRM_UPDCTRL_HPP_ */
