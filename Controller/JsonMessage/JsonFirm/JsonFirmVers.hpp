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

#ifndef JSON_FIRM_VERS_HPP_
#define JSON_FIRM_VERS_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFirmVers {
public:
    JsonFirmVers() {}
    virtual ~JsonFirmVers() {}
    static String GetStrCmd() { return "firm=vers"; }
};

typedef JsonFirmVers  JsonFirmVers_t;
typedef JsonFirmVers* JsonFirmVers_p;



#endif /* !JSON_FIRM_VERS_HPP_ */
