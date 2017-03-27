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

#ifndef JSON_DEV_INFO_HPP_
#define JSON_DEV_INFO_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevInfo : public JsonMessageBase {
public:    
    JsonDevInfo() {}
    virtual ~JsonDevInfo() {}

    static String GetStrCmd() { return "dev=info"; }
    virtual void_t Refresh() { }
};

typedef JsonDevInfo  JsonDevInfo_t;
typedef JsonDevInfo* JsonDevInfo_p;

#endif /* !JSON_DEV_INFO_HPP_ */
