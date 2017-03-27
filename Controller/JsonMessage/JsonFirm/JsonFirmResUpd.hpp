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

#ifndef JSON_FIRM_RESUPD_HPP_
#define JSON_FIRM_RESUPD_HPP_


#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFirmResUpd {
public:
    JsonFirmResUpd() {}
    virtual ~JsonFirmResUpd() {}
    static String GetStrCmd() { return "firm=resupd"; }
};

typedef JsonFirmResUpd  JsonFirmResUpd_t;
typedef JsonFirmResUpd* JsonFirmResUpd_p;


#endif /* !JSON_FIRM_RESUPD_HPP_ */
