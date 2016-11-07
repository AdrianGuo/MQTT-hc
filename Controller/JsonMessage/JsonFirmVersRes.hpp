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

#ifndef JSON_FIRM_VERSRES_HPP_
#define JSON_FIRM_VERSRES_HPP_


#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"


class JsonFirmVersRes {
public:
    JsonFirmVersRes() {}
    virtual ~JsonFirmVersRes() {}
    static String GetStrCmd() const { return "firm=versres"; }
};

typedef JsonFirmVersRes  JsonFirmVersRes_t;
typedef JsonFirmVersRes* JsonFirmVersRes_p;



#endif /* !JSON_FIRM_VERSRES_HPP_ */
