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

#ifndef JSON_DEV_OFF_HPP_
#define JSON_DEV_OFF_HPP_


#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevOff {
public:
    JsonDevOff() {}
    virtual ~JsonDevOff() {}
    static String GetStrCmd() { return "dev=off"; }
};

typedef JsonDevOff  JsonDevOff_t;
typedef JsonDevOff* JsonDevOff_p;



#endif /* !JSON_DEV_OFF_HPP_ */
