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
 * Last Changed:     Date: 2016-10-31 19:40:00 (Mon, 31 Oct 2016)
 *
 ******************************************************************************/

#ifndef JSON_ZWDELRES_HPP_
#define JSON_ZWDELRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwDelRes {
public:
    JsonZwDelRes() {}
    virtual ~JsonZwDelRes() {}

    static String GetStrCmd() { return "zw=delres"; }
};

typedef JsonZwDelRes  JsonZwDelRes_t;
typedef JsonZwDelRes* JsonZwDelRes_p;

#endif /* !JSON_ZWDELRES_HPP_ */
